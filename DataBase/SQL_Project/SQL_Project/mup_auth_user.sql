USE [Account]
GO
/****** ��ü:  StoredProcedure [dbo].[mup_auth_user]    ��ũ��Ʈ ��¥: 03/11/2009 17:48:00 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[mup_auth_user]
    @bDebugAuth [bit],
	@strUserID_ [nvarchar](20),
	@strPassword_ [nvarchar](20),
	@iAge_ [int],
	@iPublisherSN_ [int],
	@iOK [int] = 0,
	@iUserUID [bigint] = 0,
    @bPlayGuide [bit] = 1,
	@iAuthLevel [int] = 0,
    @bInternalUser [bit] = 0
AS
SET NOCOUNT ON;

DECLARE @strPassword nvarchar(20)
,       @strUserName nvarchar(16)
,		@iAge int
,       @bDeleted bit
,		@sdtEnddate smalldatetime
,		@strReason	nvarchar(100)
,		@bIsRecommend bit
,		@iPublisherSN	int
,		@iSkillOption bit


SELECT  @sdtEnddate = getdate()
,		@strReason = '���� ����'

SELECT  @iUserUID = UserUID
,       @strPassword = Password
,       @strUserName = UserName
,		@iAge = Age
,       @bPlayGuide = PlayGuide
,       @bDeleted = Deleted
,		@bIsRecommend = IsRecommend
,		@iPublisherSN = PublisherSN
    FROM dbo.MUser WITH(NOLOCK)
        WHERE UserID = @strUserID_


-- ��ų �ɼ� ����
SELECT @iSkillOption = SkillOption 
	FROM dbo.MUserSkillOption WITH (NOLOCK)
		WHERE USERUID = @iUserUID



IF @iUserUID = 0 --������ �������� ����
    BEGIN    SELECT @iOK = -1    GOTO END_PROC    END

IF @bDeleted = 1 --������ ������
    BEGIN    SELECT @iOK = -2    GOTO END_PROC    END

IF @bDebugAuth = 1
BEGIN
	IF @strPassword_ <> @strPassword --Ʋ�� �������
		BEGIN    SELECT @iOK = -3    GOTO END_PROC    END
END

-- ���̴� ���� �� �ִ�
IF @iAge_ <> @iAge
BEGIN
	UPDATE dbo.MUser SET Age = @iAge_ WHERE UserID = @strUserID_
END

SELECT @iAuthLevel = AuthLevel FROM dbo.MUserAuthority( NOLOCK ) WHERE UserUID = @iUserUID

-- �Ϲ� �������, ���� �� �����ΰ� �ľ��Ѵ�
IF @iAuthLevel = 0	
	BEGIN
		SELECT @iAuthLevel = authlevel, @sdtEndDate = enddate, @strReason = reason
			from dbo.userauthlevel with (nolock)
				where useruid = @iUserUID
	END

-- �����°� ON�� ������� EndDate�� ���ú��� ���� ��¥���� �Ǵ�
IF @iAuthLevel < 0
	BEGIN
		IF @sdtEnddate < getdate()
			BEGIN
				UPDATE dbo.userauthlevel 
					SET authlevel = 0
						WHERE userUID = @iuserUID

				SELECT @iAuthLevel = 0
			END

	END


-- �系 �Ǵ� �ؽ� �������� �ĺ��Ѵ�.
IF EXISTS( SELECT * FROM dbo.MInternalID WITH( NOLOCK ) WHERE ID = @strUserID_ )
BEGIN
    SET @bInternalUser = 1
END

END_PROC:
--������ �� �߰� 0
SELECT @iOK, @iUserUID, @strUserName, @iAuthLevel, @bPlayGuide, @bInternalUser, @sdtEnddate, @strReason, @bIsRecommend, ISNULL(@iSkillOption, 0)
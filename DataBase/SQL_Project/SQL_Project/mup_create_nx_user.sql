USE [Account]
GO
/****** ��ü:  StoredProcedure [dbo].[mup_create_nx_user]    ��ũ��Ʈ ��¥: 03/11/2009 17:48:48 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
ALTER PROCEDURE [dbo].[mup_create_nx_user]
	@strUserID_ [nvarchar](20),
	@strPassword_ [nvarchar](20),
    @iPublisherSN_ [int],
    @bGender_ [bit],
    @iAge_ [tinyint],
	@strUserName_ [nvarchar](16),
	@iGuestUser_ [bit],
	@iOK [int] = 0,
	@iUserUID [bigint] = 0,
	@bPlayGuide [bit] = 1,
	@bInternalUser [bit] = 0

AS
SET NOCOUNT ON;

-- �ߺ��Ǵ� UserID
IF EXISTS( SELECT * FROM dbo.Muser WITH(NOLOCK) WHERE UserID = @strUserID_ )
BEGIN
    SELECT @iOK = -1
    GOTO END_PROC
END

DECLARE @sdtNow smalldatetime
SELECT @sdtNow = GetDate()

BEGIN TRAN
    INSERT INTO   dbo.MUser(
                            UserID
,                           Password
,                           PublisherSN
,                           Gender
,                           Age
,                           UserName
,                           RegDate
,                           DelDate
,                           PlayGuide
,							GuestUser
                           )
        SELECT  @strUserID_
,               @strPassword_
,               @iPublisherSN_
,               @bGender_
,               @iAge_
,               @strUserName_
,               @sdtNow
,               @sdtNow
,               1
,				@iGuestUser_
    IF @@ERROR <> 0 OR @@ROWCOUNT <> 1 --�̰� �Ѱ��� �ԷµǾ�� �ϻ�
    BEGIN
        SELECT @iOK = -11
        GOTO FAIL_TRAN
    END

    SELECT @iUserUID = @@IDENTITY

--	��ų ���� �ɼ� INSERT
INSERT INTO dbo.MUserSkillOption (UserUID, SkillOption)
	SELECT @iUserUID, 1

COMMIT TRAN

-- �系 �Ǵ� �ؽ� �������� �ĺ��Ѵ�.
IF EXISTS( SELECT * FROM dbo.MInternalID WITH( NOLOCK ) WHERE ID = @strUserID_ )
BEGIN
    SET @bInternalUser = 1
END

GOTO END_PROC

FAIL_TRAN:
ROLLBACK TRAN

END_PROC:
SELECT @iOK, @iUserUID, @bPlayGuide, @bInternalUser, 0, 1 -- ������ 1�� ��ų���� �ɼǰ����� default �� 1�̴�.
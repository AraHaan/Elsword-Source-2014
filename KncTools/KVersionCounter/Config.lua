
g_pMain:SetClientVersion_LUA("20070702_0" )

--g_pMain:ResetFullScreenResolution(1024, 768)
g_pMain:SetLoadBalancerIP( "59.25.186.60" )
g_pMain:SetLoadBalancerPort( 9900 )

--����
--g_pMain:AddServerIP_LUA( "211.218.230.101" )
--g_pMain:AddServerIP_LUA( "211.218.230.102" )
--g_pMain:AddServerIP_LUA( "211.218.230.103" )

g_pMain:AddServerIP_LUA( "59.25.186.60" )
--g_pMain:AddServerIP_LUA( "59.25.186.54" )
--g_pMain:AddServerIP_LUA( "59.25.186.59" )
--g_pMain:AddServerIP_LUA( "59.25.186.185" )
--g_pMain:AddServerIP_LUA( "59.25.186.176" )
--g_pMain:AddServerIP_LUA( "59.25.186.39" )
g_pMain:SetServerPort( 9300 )
g_pMain:SetServerUDPPort( 9301 )
g_pMain:SerServerTimeOut( 20 )

g_pMain:SetAutoLoginUnitIndex( 0 )
g_pMain:SetSafetyKey( "" )																		 

g_pMain:InstallFont( "���� & ����ü & ���� & ����ü (TrueType)", "gulim.ttc" )
g_pMain:InstallFont( "HY������M (TrueType)", "H2HDRM.TTF" )


g_pMain:DeleteFile( "ErrorLog.txt" )
g_pMain:DeleteFile( "DialogLog.txt" )
g_pMain:DeleteFile( "P2PLog.txt" )


-- GAME_P2P_PORT = 8765



--��Ʈ ����
g_pFontManager:CreateNewUIFont( XF_DODUM_15_NORMAL, "����", 15, FONT_WEIGHT["FW_NORMAL"]		)


--X2Define���� �����������


g_pFontManager:CreateNewUIUKFont( XUF_DODUM_11_BOLD,		"����",			11, FONT_WEIGHT["FW_BOLD"]	)
g_pFontManager:CreateNewUIUKFont( XUF_DODUM_13_SEMIBOLD,	"����",			13, FONT_WEIGHT["FW_SEMIBOLD"]	)
g_pFontManager:CreateNewUIUKFont( XUF_DODUM_15_BOLD,		"����",			15, FONT_WEIGHT["FW_BOLD"]	)
g_pFontManager:CreateNewUIUKFont( XUF_DODUM_20_BOLD,		"����",			20, FONT_WEIGHT["FW_BOLD"]	)
g_pFontManager:CreateNewUIUKFont( XUF_DODUM_30_BOLD,		"����",			30, FONT_WEIGHT["FW_BOLD"]	)

g_pFontManager:CreateNewUIUKFont( XUF_HEADLINE_30_NORMAL,	"HY������M",	30, FONT_WEIGHT["FW_NORMAL"]	)
g_pFontManager:CreateNewUIUKFont( XUF_HEADLINE_60_NORMAL,	"HY������M",	60, FONT_WEIGHT["FW_NORMAL"]	)




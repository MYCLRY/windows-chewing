!ifdef CURLANG
  !undef CURLANG
!endif
!define CURLANG ${LANG_TRADCHINESE}

!insertmacro CHEWING_LANG_BEGIN

LicenseLangString LICENSE_FILE ${CURLANG} "License.txt"

LangString NAME				${CURLANG} "�s�ŭ���J�k ${PRODUCT_VERSION}"

LangString DETECTED_OLD_VERSION		${CURLANG} "������w�w���ª�"
LangString REINSTALL_CONFIRM		${CURLANG} "�A�O�_�n�����ª����~��w�˷s���H"

LangString VISIT_WEBSITE		${CURLANG} "���[�s�ŭ���J�k�����G "

LangString INSTALL_FAIL			${CURLANG} "�w�˵o�Ϳ��~�A�нT�w�A���t�κ޲z���v���A�H���ª����b���椤$\n$\n��ĳ�챱��x��J�k�]�w���A�����ª��í��}����A�w�ˤ@���C"

LangString UNINSTALL_SUCCESS		${CURLANG} "�w���\�a�q�A���q������ $(^Name) �C"
LangString UNINSTALL_CONFIRM		${CURLANG} "�A�T�w�n�������� $(^Name) �A��ΩҦ�������H"

LangString STARTMENU_FOLDER_NAME	${CURLANG} "�s�ŭ���J�k"
LangString LNK_USER_Manual		${CURLANG} "�s�ŭ���J�k�ϥλ���"
LangString LNK_DB_EDITOR		${CURLANG} "�ϥΪ̵��w�s��u��"
LangString LNK_UPDATE_CHECKER		${CURLANG} "�u�W�ˬd�O�_���s����"
LangString LNK_UNINSTALL		${CURLANG} "�Ѱ��w��"

!insertmacro CHEWING_LANG_END

#ifndef __NDM_H_
#define __NDM_H_

#define NESEP_					NDM_FEEDBACK_ENV_SEPARATOR

#define NDM_OPENVPN_DIR			"/tmp/openvpn/"

#define NDM_FEEDBACK_NETWORK	"network.feedback"

#define NDM_INSTANCE_NAME		(pgmname_syslog == NULL ? "_ndm_null_" : pgmname_syslog)

#define NDM_ADD					"add4"
#define NDM_ADD6				"add6"
#define NDM_DEL					"del4"
#define NDM_DEL6				"del6"

#define NDM_IFCONFIG			"ifconfig"
#define NDM_LLADDR				"lladdr"
#define NDM_ROUTE				"route"

#endif /* __NDM_H_ */

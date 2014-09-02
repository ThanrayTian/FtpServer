#include "session.h"
#include "common.h"
#include "ftp_nobody.h"
#include "ftp_proto.h"
#include "priv_sock.h"

void session_init(session_t *sess)
{
	memset(sess->command, 0, sizeof (sess->command));
	memset(sess->comm, 0, sizeof (sess->comm));
	memset(sess->args, 0, sizeof (sess->args));
	sess->peerfd = -1;
	sess->nobody_fd = -1;
	sess->proto_fd = -1;

	sess->user_uid = 0;
	sess->ascii_mode = 0;

	sess->p_addr = NULL;
	sess->data_fd = -1;
	sess->listen_fd = -1;
}

void session_reset_command(session_t *sess)
{
	memset(sess->command, 0, sizeof (sess->command));
	memset(sess->comm, 0, sizeof (sess->comm));
	memset(sess->args, 0, sizeof (sess->args));
}

void session_begin(session_t *sess)
{
	priv_sock_init(sess);

	pid_t pid;
	if((pid = fork()) == -1)
		ERR_EXIT("fork");
	else if(pid == 0)
	{
		priv_sock_set_proto_context(sess);
		handle_proto(sess);
	}
	else
	{
		priv_sock_set_nobody_context(sess);
		handle_nobody(sess);
	}

}
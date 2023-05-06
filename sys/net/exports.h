#ifndef _NET_EXPORTS_H_
#define _NET_EXPORTS_H_

struct export_args;
struct mount;
struct netcred;
struct netexport;
struct sockaddr;
struct ucred;

int	exports_check(struct mount *, struct sockaddr *, uint64_t *,
	    struct ucred **, int *, int *);
void	exports_free(struct mount *);
void	exports_free_addrlist(struct netexport *);
struct netcred *exports_lookup(struct mount *, struct sockaddr *);
int	exports_set_options(struct mount *, struct export_args *);

#endif /* _NET_EXPORTS_H_ */

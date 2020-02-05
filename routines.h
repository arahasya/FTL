/* Pi-hole: A black hole for Internet advertisements
*  (c) 2017 Pi-hole, LLC (https://pi-hole.net)
*  Network-wide ad blocking via your own hardware.
*
*  FTL Engine
*  Global prototypes
*
*  This file is copyright under the latest version of the EUPL.
*  Please see LICENSE file for your rights under this license. */

void go_daemon(void);
void timer_start(int i);
double timer_elapsed_msec(int i);
void sleepms(int milliseconds);
void savepid(void);
char * getUserName(void);
void removepid(void);

void open_FTL_log(bool test);
void logg(const char* format, ...) __attribute__ ((format (gnu_printf, 1, 2)));
void logg_struct_resize(const char* str, int to, int step);
void log_counter_info(void);
void format_memory_size(char *prefix, unsigned long int bytes, double *formated);
void log_FTL_version(bool crashreport);

// datastructure.c
void strtolower(char *str);
int findForwardID(const char * forward, bool count);
int findDomainID(const char *domain);
int findClientID(const char *client, bool addNew);
bool isValidIPv4(const char *addr);
bool isValidIPv6(const char *addr);
const char *getDomainString(int queryID);
const char *getClientIPString(int queryID);
const char *getClientNameString(int queryID);

void close_telnet_socket(void);
void close_unix_socket(void);
void seom(int sock);
void ssend(int sock, const char *format, ...) __attribute__ ((format (gnu_printf, 2, 3)));
void swrite(int sock, const void* value, size_t size);
void *telnet_listening_thread_IPv4(void *args);
void *telnet_listening_thread_IPv6(void *args);

void *socket_listening_thread(void *args);
bool ipv6_available(void);
void bind_sockets(void);

void process_request(const char *client_message, int *sock);
bool command(const char *client_message, const char* cmd) __attribute__((pure));
bool matchesEndpoint(char *client_message, const char *cmd);

// grep.c
int countlines(const char* fname);
int countlineswith(const char* str, const char* fname);
void check_blocking_status(void);

void check_setupVarsconf(void);
char * read_setupVarsconf(const char * key);
void getSetupVarsArray(const char * input);
void clearSetupVarsArray(void);
bool insetupVarsArray(const char * str);
bool getSetupVarsBool(const char * input) __attribute__((pure));

void parse_args(int argc, char* argv[]);

// setupVars.c
char* find_equals(const char* s) __attribute__((pure));
void trim_whitespace(char *string);

// config.c
void getLogFilePath(void);
void read_FTLconf(void);
void get_privacy_level(FILE *fp);
void get_blocking_mode(FILE *fp);
void read_debuging_settings(FILE *fp);

// gc.c
void *GC_thread(void *val);

// database.c
void db_init(void);
void *DB_thread(void *val);
int get_number_of_queries_in_DB(void);
void save_to_DB(void);
void read_data_from_DB(void);
bool db_set_FTL_property(unsigned int ID, int value);
bool dbquery(const char *format, ...);
bool dbopen(void);
void dbclose(void);
int db_query_int(const char*);
void SQLite3LogCallback(void *pArg, int iErrCode, const char *zMsg);

// memory.c
void memory_check(int which);
char *FTLstrdup(const char *src, const char *file, const char *function, int line) __attribute__((malloc));
void *FTLcalloc(size_t nmemb, size_t size, const char *file, const char *function, int line) __attribute__((malloc)) __attribute__((alloc_size(1,2)));
void *FTLrealloc(void *ptr_in, size_t size, const char *file, const char *function, int line) __attribute__((alloc_size(2)));
void FTLfree(void *ptr, const char* file, const char *function, int line);
void validate_access(const char * name, int pos, bool testmagic, int line, const char * function, const char * file);

int main_dnsmasq(int argc, const char ** argv);

// signals.c
void handle_signals(void);

// resolve.c
void *DNSclient_thread(void *val);
void resolveClients(bool onlynew);
void resolveForwardDestinations(bool onlynew);

// regex.c
bool match_regex(char *input);
void free_regex(void);
void read_regex_from_file(void);
bool in_whitelist(char *domain) __attribute__((pure));

// shmem.c
bool init_shmem(void);
void destroy_shmem(void);
size_t addstr(const char *str);
const char *getstr(size_t pos);
void *enlarge_shmem_struct(char type);

/**
 * Create a new overTime client shared memory block.
 * This also updates `overTimeClientData`.
 */
void newOverTimeClient(int clientID);

/**
 * Add a new overTime slot to each overTime client shared memory block.
 * This also updates `overTimeClientData`.
 */
void addOverTimeClientSlot(void);

// overTime.c
void initOverTime(void);
unsigned int getOverTimeID(time_t timestamp);

/**
 * Move the overTime slots so the oldest interval starts with mintime. The time
 * given will be aligned to OVERTIME_INTERVAL.
 *
 * @param mintime The start of the oldest interval
 */
void moveOverTimeMemory(time_t mintime);

// capabilities.c
bool check_capabilities(void);

// networktable.c
bool create_network_table(void);
void parse_arp_cache(void);
void updateMACVendorRecords(void);

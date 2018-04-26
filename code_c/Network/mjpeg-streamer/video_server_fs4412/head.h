#ifndef _HEAD_H_
#define _HEAD_H_ 
/*
 *listen_fd:监听套接字文件描述符
 *www_dir  :网页所在目录
 *tid      :线程TID
 */
typedef struct 
{
	int listen_fd;
	char *www_dir;
	pthread_t tid;
}server_t;

/*
 *stop  :0表示不采集视频数据，1表示采集视频数据
 *start :存放视频数据的起始地址
 *length:视频数据的长度
 *update_lock:线程锁
 *update_cond:线程条件变量
 */
typedef struct 
{
	int stop;
	void *start;
	int length;
	pthread_mutex_t buffer_lock;
	pthread_mutex_t update_lock;
	pthread_cond_t  update_cond;
	pthread_cond_t  open_cond;
	pthread_mutex_t open_lock;
}global_t;

extern global_t global;

#define WIDTH  640
#define HEIGHT 480

//...
typedef struct
{
	void *start;
	int length;
}camera_t;


#define BUFFER_SIZE 1024

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define LENGTH_OF(x) (sizeof(x)/sizeof(x[0]))

/* the webserver determines between these values for an answer */
typedef enum { A_UNKNOWN, A_SNAPSHOT, A_STREAM, A_COMMAND, A_FILE } answer_t;

//HTTP 请求
typedef struct 
{
	answer_t type;
	char *parm;
}request_t;

//client thread info 
typedef struct 
{
	int fd;
	char *www_dir;
	int connect_fd;
	pthread_t tid;
}client_t;

/*
 * Standard header to be send along with other header information like mimetype.
 * The parameters should ensure the browser does not cache our answer.
 * A browser should connect for each file and not serve files from his cache.
 * Using cached pictures would lead to showing old/outdated pictures
 * Many browser seem to ignore, or at least not always obey those headers
 * since i observed caching of files from time to time.
 */
#define STD_HEADER "Connection: close\r\n" \
                   "Server: MJPG-Streamer/0.2\r\n" \
                   "Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\n" \
                   "Pragma: no-cache\r\n" \
                   "Expires: Mon, 3 Jan 2013 12:34:56 GMT\r\n"


/* the boundary is used for the M-JPEG stream, it separates the multipart stream of pictures */
#define BOUNDARY "cyg-boundary"

/*
 * Only the following fileypes are supported.
 *
 * Other filetypes are simply ignored!
 * This table is a 1:1 mapping of files extension to a certain mimetype.
 */
static const struct {
  const char *dot_extension;
  const char *mimetype;
} mimetypes[] = {
  { ".html", "text/html" },
  { ".htm",  "text/html" },
  { ".css",  "text/css" },
  { ".js",   "text/javascript" },
  { ".txt",  "text/plain" },
  { ".jpg",  "image/jpeg" },
  { ".jpeg", "image/jpeg" },
  { ".png",  "image/png"},
  { ".gif",  "image/gif" },
  { ".ico",  "image/x-icon" },
  { ".swf",  "application/x-shockwave-flash" },
  { ".cab",  "application/x-shockwave-flash" },
  { ".jar",  "application/java-archive" }
};

extern void *capture_stream(void *arg);
extern  void *accept_client_request(void *arg);

#endif 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>

#define CLEAR(x) memset (&(x), 0, sizeof (x))

struct buffer {
  void * start;
  size_t length;
};

static char * dev_name = "/dev/video0";//摄像头设备名
static int fd = -1;
struct buffer * buffers = NULL;
static unsigned int n_buffers = 0;

FILE *file_fd;
static unsigned long file_length;
static unsigned char *file_name;
//////////////////////////////////////////////////////
//获取一帧数据
//////////////////////////////////////////////////////
static int read_frame (void)
{
  struct v4l2_buffer buf;
  unsigned int i;

  CLEAR (buf);
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  //11.出列采集的帧缓冲
  ioctl (fd, VIDIOC_DQBUF, &buf); 

  assert (buf.index < n_buffers);
  printf("buf.index dq is %d,\n",buf.index);

  //12.将其写入文件中
  fwrite(buffers[buf.index].start, buf.bytesused, 1, file_fd); 
	printf("xxx---->len_frame = %d\n",buf.bytesused);
  //13.再将其入列
  ioctl (fd, VIDIOC_QBUF, &buf); 

  return 1;
}

//播放yuyv422格式数据
//# ffplay -i test.yuv -pix_fmt yuyv422 -s 1280x720

int main (int argc,char ** argv)
{
  struct v4l2_capability cap;
  struct v4l2_format fmt;
  unsigned int i;
  enum v4l2_buf_type type;
  //while(1){
	file_fd = fopen("test.yuv", "w");//图片文件名
  //1.打开设备
  fd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);
  printf("%s: %d: fd = %d (%s) \n",__func__,__LINE__,fd,strerror(errno));

  {//获取摄像头所支持的分辨率
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    struct v4l2_fmtdesc fmt;
    struct v4l2_frmsizeenum frmsize;
    struct v4l2_frmivalenum frmival;

    fmt.index = 0;
    fmt.type = type;
    while (ioctl(fd, VIDIOC_ENUM_FMT, &fmt) >= 0) {
      frmsize.pixel_format = fmt.pixelformat;
      frmsize.index = 0;
      while (ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize) >= 0) {
				if (frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
					printf("line:%d %dx%d\n",__LINE__, frmsize.discrete.width, frmsize.discrete.height);
				} else if (frmsize.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
					printf("line:%d %dx%d\n",__LINE__, frmsize.discrete.width, frmsize.discrete.height);
				}
				frmsize.index++;
      }
      fmt.index++;
    }

  }

  //2.获取摄像头参数
  ioctl(fd, VIDIOC_QUERYCAP, &cap);
	// printf("%s: %d: fd = %d (%s) \n",__func__,__LINE__,fd,strerror(errno));

  CLEAR (fmt);
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width =  1280;
  fmt.fmt.pix.height = 720;
	//H264 Encode
  //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_H264;
	
	//YUYV Encode
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  fmt.fmt.pix.field = V4L2_FIELD_INTERLACED; 
  //printf("%s: %d: (%s) \n",__func__,__LINE__,strerror(errno));
  //3.设置图像格式
  ioctl (fd, VIDIOC_S_FMT, &fmt); 
  //printf("%s: %d: fd = %d (%s) \n",__func__,__LINE__,fd,strerror(errno));
  file_length = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height; //计算图片大小

  struct v4l2_requestbuffers req;
  CLEAR (req);
  req.count = 4;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  //5.申请缓冲，count是申请的数量
  ioctl (fd, VIDIOC_REQBUFS, &req); 

  if (req.count < 2)
    printf("Insufficient buffer memory\n");
  //6.内存中建立对应空间
  buffers = calloc (req.count, sizeof (*buffers));

  for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
    {
      struct v4l2_buffer buf; //驱动中的一帧
      CLEAR (buf);
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory = V4L2_MEMORY_MMAP;
      buf.index = n_buffers;
      //7.映射用户空间
      if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf))
				printf("VIDIOC_QUERYBUF error\n");

      buffers[n_buffers].length = buf.length;
      buffers[n_buffers].start = mmap (NULL /* start anywhere */, //通过mmap建立映射关系
																			 buf.length,
																			 PROT_READ | PROT_WRITE /* required */,
																			 MAP_SHARED /* recommended */,
																			 fd, buf.m.offset);

      if (MAP_FAILED == buffers[n_buffers].start)
				printf("mmap failed\n");
    }

  for (i = 0; i < n_buffers; ++i)
    {
      struct v4l2_buffer buf;
      CLEAR (buf);

      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory = V4L2_MEMORY_MMAP;
      buf.index = i;
      //8.申请到的缓冲进入列队
      if (-1 == ioctl (fd, VIDIOC_QBUF, &buf))
				printf("VIDIOC_QBUF failed\n");
    }

  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  //9.开始捕捉图像数据
  if (-1 == ioctl (fd, VIDIOC_STREAMON, &type))
    printf("VIDIOC_STREAMON failed\n");
  
  for (i = 0; i < 1; i++) //这一段涉及到异步IO
    {
      fd_set fds;
      struct timeval tv;
      int r;

      FD_ZERO (&fds);//将指定的文件描述符集清空
      FD_SET (fd, &fds);//在文件描述符集合中增加一个新的文件描述符

      /* Timeout. */
      tv.tv_sec = 2;
      tv.tv_usec = 0;

      r = select (fd + 1, &fds, NULL, NULL, &tv);//判断是否可读（即摄像头是否准备好），tv是定时

      if (-1 == r) {
				if (EINTR == errno)
					continue;
				printf("select err\n");
      }
      if (0 == r) {
				fprintf(stderr, "select timeout\n");
				exit (EXIT_FAILURE);
      }
      //10.如果可读，执行read_frame ()函数，并跳出循环
      //while(1){
      if(read_frame()){
				//break;
      }
      //}
    }

 unmap:
  for (i = 0; i < n_buffers; ++i)
    if (-1 == munmap (buffers[i].start, buffers[i].length))
      printf("munmap error");
  close (fd);
  fclose (file_fd);
  exit (EXIT_SUCCESS);
  return 0;
  //}
}

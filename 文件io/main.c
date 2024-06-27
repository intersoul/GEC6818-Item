#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int ret;
	int fd;
	unsigned long file_size;
	char wr_buf[100] = "hello world,my name is caiji";
	char rd_buf[100];

	fd = open("test1.txt", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IROTH); // 打开文件
	if (fd < 0)
	{
		printf("open file error...\r\n");
		return -1;
	}

	ret = write(fd, wr_buf, sizeof(wr_buf)); // 写入数据
	if (ret < 0)
	{
		printf("write file error...\r\n");
		return -1;
	}

	lseek(fd, 0, SEEK_SET);

	ret = read(fd, rd_buf, sizeof(rd_buf)); // 读取数据
	if (ret < 0)
	{
		printf("read file error...\r\n");
		return -1;
	}

	printf("rd_buf = %s\r\n", rd_buf); // 打印数据

	file_size = lseek(fd, 0, SEEK_END); // 获取指定文件的当前位置偏移量
	if (file_size < 0)
	{
		printf("file_size error...\r\n");
	}

	printf("file_size = %lu\n", file_size); // 打印当前文件大小

	close(fd); // 关闭文件

	return 0;
}

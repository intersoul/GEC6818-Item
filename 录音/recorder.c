#include "head4audio.h"

// 根据本系统的具体字节序处理的存放格式
#if   __BYTE_ORDER == __LITTLE_ENDIAN

	#define RIFF ('F'<<24 | 'F'<<16 | 'I'<<8 | 'R'<<0)
	#define WAVE ('E'<<24 | 'V'<<16 | 'A'<<8 | 'W'<<0)
	#define FMT  (' '<<24 | 't'<<16 | 'm'<<8 | 'f'<<0)
	#define DATA ('a'<<24 | 't'<<16 | 'a'<<8 | 'd'<<0)

	#define LE_SHORT(val) (val) 
	#define LE_INT(val)   (val) 

#elif __BYTE_ORDER == __BIG_ENDIAN

	#define RIFF ('R'<<24 | 'I'<<16 | 'F'<<8 | 'F'<<0)
	#define WAVE ('W'<<24 | 'A'<<16 | 'V'<<8 | 'E'<<0)
	#define FMT  ('f'<<24 | 'm'<<16 | 't'<<8 | ' '<<0)
	#define DATA ('d'<<24 | 'a'<<16 | 't'<<8 | 'a'<<0)

	#define LE_SHORT(val) bswap_16(val) 
	#define LE_INT(val)   bswap_32(val) 

#endif

#define DURATION_TIME 3 //录音时间
int t;

void prepare_wav_params(wav_format *wav);
void set_wav_params(pcm_container *sound, wav_format *wav);
void recorder(int fd, pcm_container *sound, wav_format *wav);
snd_pcm_sframes_t read_pcm_data(pcm_container *sound, snd_pcm_uframes_t frames);

int main(int argc, char const *argv[])
{
	if (argc != 3)
	{
		printf("Usage: %s <wav-file> time\n", argv[0]);
		exit(1);
	}
	t = atoi(argv[2]);

	//1.打开wav文件，用来存放录制好的音频数据
	int wav_fd = open(argv[1], O_CREAT|O_WRONLY|O_TRUNC, 0777);
	if (wav_fd == -1)
	{
		perror("open() error");
		exit(1);
	}

	//打开PCM设备文件
	//PCM的结构体变量
	pcm_container *sound = calloc(1, sizeof(pcm_container));
	/*参数1：要拿到的句柄
	  参数2：名字自定，
	  参数3：PCM流方向：
	  		SND_PCM_STREAM_CAPTURE：录音
	  		SND_PCM_STREAM_PLAYBACK：播放
	  参数4：打开模式 0:
	  		表示标准打开模式
	  		SND_PCM_NONBLOCK:表示非阻塞模式
	  		SND_PCM_ASYNC:表示声卡系统以异步方式工作
	*/
	int ret = snd_pcm_open(&sound->handle, "default", SND_PCM_STREAM_CAPTURE, 0);
	if (ret != 0)
	{
		perror("snd_pcm_open() error");
		exit(1);
	}

	//2.设置录音文件的格式和PCM的参数
	wav_format *wav = calloc(1, sizeof(wav_format));
	//准备WAV格式
	prepare_wav_params(wav);
	//设置WAV格式和PCM的参数
	set_wav_params(sound, wav);
	
	//3.录音：从音频输入读取——>写入文件
	recorder(wav_fd, sound, wav);

	//4.关闭文件释放资源
	close(wav_fd);
	snd_pcm_drain(sound->handle);
	snd_pcm_close(sound->handle);
	free(sound->period_buf);
	free(sound);
	free(wav);

	return 0;
}

//准备wav格式
void prepare_wav_params(wav_format *wav)
{
	wav->head.id = RIFF;	// 固定为'RIFF'
	wav->head.size = LE_INT(36 + wav->data.data_size);	// 除了id和size之外，整个WAV文件的大小
	wav->head.format = WAVE;// fmt chunk的格式，此处为'WAVE'

	wav->format.fmt_id = FMT;	// 固定为'fmt '
	wav->format.fmt_size = LE_INT(16);	// 在fmt chunk中除了fmt_id和fmt_size之外的大小，固定为16字节
	wav->format.fmt = LE_SHORT(WAV_FMT_PCM);		// data chunk中数据的格式代码，PCM的代码是0x0001
	wav->format.channels = LE_SHORT(1);	// 声道数目，由用户设置：1为单声道，2为立体声
	wav->format.sample_rate = LE_INT(16000);	// 采样频率，由用户设置：典型值是11025Hz、22050Hz和44100Hz

	wav->format.bits_per_sample = LE_SHORT(16);	// 量化位数，由用户设置：典型值是8、16、32
	wav->format.block_align = LE_SHORT(wav->format.channels * wav->format.bits_per_sample / 8);		// 帧大小 = 声道数 * 量化级/8
	wav->format.byte_rate = LE_INT(wav->format.sample_rate * wav->format.block_align);		// 码率 = 采样率 * 帧大小

	wav->data.data_id = DATA;	// 固定为'data'
	wav->data.data_size = LE_INT(t * wav->format.byte_rate);	// 除了WAV格式头之外的音频数据大小
}

//设置wav格式和PCM设备的参数
void set_wav_params(pcm_container *sound, wav_format *wav)
{
	//1.定义PCM设备的参数结构体指针并初始化
	snd_pcm_hw_params_t *hwparams;
	snd_pcm_hw_params_alloca(&hwparams);
	snd_pcm_hw_params_any(sound->handle, hwparams);
	//2.设置访问模式:交错模式（帧连续）
	snd_pcm_hw_params_set_access(sound->handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
	//3.设置量化参数
	snd_pcm_format_t pcm_format = SND_PCM_FORMAT_S16_LE;
	snd_pcm_hw_params_set_format(sound->handle, hwparams, pcm_format);
	sound->format = LE_SHORT(wav->format.channels);
	//4.音轨数量
	uint16_t channels = LE_SHORT(wav->format.channels);
	snd_pcm_hw_params_set_channels(sound->handle, hwparams, channels);
	sound->channels = LE_SHORT(wav->format.channels);
	//5.设置采样频率
	uint32_t exact_rate = LE_INT(wav->format.sample_rate);
	snd_pcm_hw_params_set_rate_near(sound->handle, hwparams, &exact_rate, NULL);
	//6.设置buffer size为声卡支持最大值
	snd_pcm_uframes_t buffer_size;
	snd_pcm_hw_params_get_buffer_size_max(hwparams, &buffer_size);
	snd_pcm_hw_params_set_buffer_size_near(sound->handle, hwparams, &buffer_size);
	//7.设置period size
	snd_pcm_uframes_t period_size = buffer_size/4;
	snd_pcm_hw_params_set_period_size_near(sound->handle, hwparams, &period_size, NULL);
	//8.安装这些PCM设备参数
	snd_pcm_hw_params(sound->handle, hwparams);

	//9.获取buffer size 和 period size
	snd_pcm_hw_params_get_buffer_size(hwparams, &sound->frames_per_buffer);
	snd_pcm_hw_params_get_period_size(hwparams, &sound->frames_per_period, NULL);
	//10.保留一些参数
	sound->bits_per_sample = snd_pcm_format_physical_width(pcm_format);
	sound->bytes_per_frame = sound->channels * sound->bits_per_sample / 8;
	//11.分配周期缓冲数据空间
	sound->period_buf = (uint8_t *)calloc(1, sound->frames_per_period * sound->frames_per_buffer);
}

//录音
void recorder(int fd, pcm_container *sound, wav_format *wav)
{
	//(1)写入wav格式文件头
	write(fd, &wav->head, sizeof(wav->head));
	write(fd, &wav->format, sizeof(wav->format));
	write(fd, &wav->data, sizeof(wav->data));

	//(2)写入PCM数据
	//total_bytes : 音频数据的字节数
	uint32_t total_bytes = wav->data.data_size;

	while(total_bytes > 0)
	{
		//total_frames : 音频数据的帧数
		uint32_t total_frames = total_bytes / sound->bytes_per_frame;
		//如果要读的数据量大于一个周期，每次按周期读取，最后剩下多少就再读多少
		snd_pcm_uframes_t n = MIN(total_frames, sound->frames_per_period);

		//[1]先从PCM中读取音频数据
		uint32_t frames_read = read_pcm_data(sound, n);
		//[2]将数据写入wav文件
		write(fd, sound->period_buf, frames_read * sound->bytes_per_frame);

		//每次写入数都相应的减去已经写入的数据量
		total_bytes -= frames_read * sound->bytes_per_frame;
	}
}

//从PCM读取数据,参数1：pcm结构体指针， 参数2：想要读到多少帧数据，返回值：实际读到的帧数
snd_pcm_sframes_t read_pcm_data(pcm_container *sound, snd_pcm_uframes_t frames)
{
	//定义一个实际读到的数据帧数，初始化为0
	snd_pcm_sframes_t exact_frames = 0;
	//定义一个每次读到的帧数
	snd_pcm_sframes_t n = 0;
	//缓冲区
	uint8_t *p = sound->period_buf;
	//如果没有读到想要的数据量，继续读
	while(frames > 0)
	{
		//参数1：pcm设备文件句柄， 参数2：缓冲区起始地址，参数3：预计读到的数据量
		n = snd_pcm_readi(sound->handle, p, frames);
		//还剩下多少没读
		frames -= n;
		//读到的帧数
		exact_frames += n;
		//起始地址偏移
		p += n*sound->bytes_per_frame;
	}

	return exact_frames;
}
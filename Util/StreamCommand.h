#ifndef __STREAM_COMMAND_H_INCLUDED__
#define __STREAM_COMMAND_H_INCLUDED__

/*
����� ������.
��� ������ ������ ������ ���� ���� ����������� �������.
����� execute() ����� ���������.
*/

typedef void (*StreamDataFunction)(void* data);

class StreamDataManager
{
	XBuffer stream_;
	MTSection lockSection_;

	typedef unsigned short int DataSize;
	unsigned int sizePosition_;

	void calcSize(){
		if(sizePosition_){
			*((DataSize*)&stream_[(int)sizePosition_]) = DataSize(stream_.tell() - sizePosition_ - sizeof(DataSize));
			sizePosition_ = 0;
		}
	}
	
public:
	StreamDataManager() : stream_(1024, 1), sizePosition_(0) {}
	~StreamDataManager() { dassert(stream_.tell() == 0); }
	void execute();
	void clear(){ stream_.set(0); sizePosition_ = 0; }

	void lock(){ lockSection_.lock(); }
	void unlock(){ lockSection_.unlock(); }

	StreamDataManager& set(StreamDataFunction func);
	StreamDataManager& operator<<(const XBuffer& xb);
	StreamDataManager& operator<<(StreamDataManager& s);
	
	template<class T>
	StreamDataManager& operator<<(const T& v){
		xxassert(lockSection_.locked(), "put command without lock");
		//stream_.write(&v, sizeof(v));
		stream_.write(v);
		return *this;
	}
};

// ����� ��� ������ ����������, ����� ������ � ����� ����� ������
extern StreamDataManager uiStreamCommand;
// ����� ��� �������� ������ � ������ �� ������������ ������� �������
// ����� �������������� ����� ������ � ������������
extern StreamDataManager uiStreamGraph2LogicCommand;

#endif //__STREAM_COMMAND_H_INCLUDED__

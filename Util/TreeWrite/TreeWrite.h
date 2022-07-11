#ifndef _TREEWRITE_H_
#define _TREEWRITE_H_


//!!!!!!!! �� ������� �������������� ������������! 

/*
����� ����� - ��� ����� ���������������� ������ ��� �� ������� ������ (�� 50 ��).
����� 2 ������.
1. ���� ������.
2. �������� ���������� ��������.
3. ������ ���������� �� 1 ������.

!!!!!������� struct_name � type_name ����� ����� uint �� ��� ����������, 
	� name ����� ��� �������, ������ ��� �� �������������� ������ �� ���.

����, ����� � array ��������� ���� 4 �����, ��� ��� ���������� � ���� ������ �����������.


���������� ���������� �������� - 
	������ ��������� ����� ���������� � 4 ���� ������, ��� � ����������. �� ���� ��������� �������� �����.
	��� ��� ������������ ��������� ���������� � 2 ���� ������ ������������� ���������.

	���� ������� ��� ��������� ������ ������ ������ ������������ �������������. ��� ��� �� ���� ����� � ������.

��� ���������� ��-�� ������ ����������.
���� ������ ������ ����������, ������ ������ ����� � ����,
�� ������ ����� ������, �� �� �� ������ ����������, 
��� ��������� ��������� �������� ���� ��������� ��� ������������ ������ ������ :(


�� ����� - ��������� ���������� ������� RegisterType, 
	�������� ��� - ��� ��������� ������ � ��������� �� ��� ����� ��� �������� �������, 
	������� ������ ��� �������.

	��������� ���������� ��������� ��������� ��� ������, �������� 
		����� ���������� ��������� �� �������, � ���� � ����������� ��� ��������� ���������, �� ���/��� ���������.
		
		�� � ������� ������������� ������� - ������ ��������� ������� ����������� �������� ���������.

������� � ��� ��������� ����������������, ������� ����� ������ ������� ���� ���������� ��� pointer � ��.
	����� �� open_ptr(const char* name,const char* type_name), ������� type_name, ���� ����� ����� �������,
	��� ������ ������.

��� ����� char* ��� ������ �������� ������/������ ������ ��� ����� ���������� ����.
����� ��� ����� uint32, ��� ����� ������ �������, ��� 
����� ����������� ������������ ����� ������ � ������.
	�������� ��� 
#define STATIC_HASH(s,n) s,n
	STATIC_HASH("name",12345) 
	12345 - ������������ � �����������, ����������� ������� �� ���� *.cpp,*.h ������ ������� �� *.vcproj,*.sln

#define AUTO_HASH(s,n) save(s,#s,n)

!!!!!!��! ���� � ����� �������� ������ �������� �� ���.
� ������ - ��� ������ ������ ���� ��������� ��������� � ������������������ ������ ���������,
�� ��� ��������� ������� �������� ��������������� ���������� ������ � ��!, � ��� ������ ������� ����� 
� assert ����� ������.

/////////////////
��� ��������������� - 
����� ������ ����� ������ �������, � ������� ��� ������
���� ������ ��� ��������, �� ������ ��������� �� ��, � �������.
//////

!!����� - ����� ������� � ���� ����� ������ �������, 
  ��� ��� ����� ����� ������� ������������ � ����� ��������� ����� ������.
*/
#include "ReadWriteAdapter.h"
#include "MemoryBuffer.h"
#include <map>


enum TreeBuildInType
{
	TBIT_NONE=0,
	TBIT_BOOL=1,
	TBIT_CHAR,
	TBIT_UCHAR,
	TBIT_SHORT,
	TBIT_USHORT,
	TBIT_INT,
	TBIT_UINT,
	TBIT_FLOAT,
	TBIT_DOUBLE,
	TBIT_STRING,//string ������������ ��� char[], ��� ������ ���� ��� ������
	TBIT_MAX,
};

class TreeWrite
{
public:
	typedef unsigned int POINTER_TYPE;

	TreeWrite(FileReadWriteFactory* writer=&default_read_write);

	void refrain_from_repetition_string(bool enable){repetition_string=enable;}
	/*
		��������� � ����������� ������� ����� ���������� ���������� ������������������ ������ ������.
	*/
	void open_struct(const char* type_name);
	void close_struct();

	//� ������� ��� �������� ������ ����, � ������� ������ �� ��������� ������������.
	//� ������ ������� ������� ��� ������.
	void open_array(const char* name,const char* type_name);
	void open_array(const char* name,TreeBuildInType type);
	POINTER_TYPE close_array();

	//declare_data ��������� ������ ������/��������� �������� ������ ���� ���������, � �� �� ���� ������� ���������.
	void declare_data(const char* name,const char* type_name);
	//���� type_name==0, �� ������ ��������� �����������������, � ����� ���� ������ �������
	//������� ������� ����� � �����.
	void write_data(const void* data,int size,const char* name,const char* type_name);
	void write_data(const void* data,const char* name,TreeBuildInType type);

	//������ ������� �� ���� ���������, ����� ������ ������ ��� ������� num ��� write_data, �� ������ � ��� ���
	void write_data_array(const void* data,int size,const char* type_name,int num);
	void write_data_array(const void* data,TreeBuildInType type,int num);

	//open_ptr ��������� ������ ������/��������� �� �������� ������ ���� ���������, � �� �� ���� ������� ���������.
	void open_ptr(const char* name,const char* type_name);
	void close_ptr();

	void write_string(const char* name,const string& str);

	void close();

	void write_all(const char* file_name);
	void dump_structure();

	enum ObjectTypeIsPointer
	{
		OTIP_NOT_POINTER=0,
		OTIP_POINTER,
		OTIP_ARRAY,
	};

	//��� ������ ���������� RegisterType � ���� ��� ���� �������� ����� ���, ��� ������� ������������� ��� �� ����.
	bool prev_write_declare_new_type()const{return prev_write_declare_new_type_;};
protected:
	FileReadWriteFactory* writer_;
	MemoryBuffer stack_write;//������������ ��������� ������, ����� seek � tell
	MemoryBuffer data_write;//������� ���������� ������, � �����������
	MemoryBuffer structure;//�������� ��������

	bool repetition_string;
	map<string,POINTER_TYPE> repetition_string_map;


	struct ObjectType
	{
		string type_name;
		POINTER_TYPE size;
		bool closed;//������ ��������� ������� ��� ���������� ����������.
		ObjectTypeIsPointer is_pointer;
		POINTER_TYPE save_id;

		struct Child
		{
			ObjectType* type;
			POINTER_TYPE offset;
			string name;
		};
		vector<Child> child;

		void close()
		{
			xassert(size==0 && !closed);
			closed=true;
			for(vector<Child>::iterator it=child.begin();it!=child.end();it++)
			{
				size+=it->type->size;
			}
		}

		ObjectType():size(0),closed(false),is_pointer(OTIP_NOT_POINTER),save_id(-1){}
	};

	struct ObjectStack
	{
		ObjectType* type;
		size_t offset;//����� ��� ��� ������ ������ stack_write.tell()
		union {
			int current_sub_object;
			POINTER_TYPE last_pointer;
		};

		union {
			int current_sub_offset;
			POINTER_TYPE last_pointer_type;
		};
		bool is_array() {return type->is_pointer==OTIP_ARRAY;};
		bool is_pointer() {return type->is_pointer==OTIP_POINTER;};
	};

	vector<ObjectType> buildin_types;

	typedef map<string,ObjectType> TypesMap;
	TypesMap types;

	POINTER_TYPE current_save_id;
	vector<ObjectStack> object_stack;

	POINTER_TYPE globals_begin;
	bool prev_write_declare_new_type_;

	friend class TreeRead;
	enum
	{
		FLAG_IS_BUILDIN=0x80000000,
		FULL_POINTER_SIZE=sizeof(POINTER_TYPE)*2,
	};

	ObjectType* RegisterType(int size,const char* type_name,ObjectTypeIsPointer is_ptr,bool is_solid);

	void declare(int size,const char* name,const char* type_name,ObjectTypeIsPointer is_pointer,bool is_solid);
	void declare(const char* name,ObjectType* type);

	void write_buffer(WriteAdapter* f,MemoryBuffer& buf);
	void write_structure(WriteAdapter* f);
	void open_struct(ObjectType* type);
	void open_struct(const char* type_name,bool is_array);
	void close_struct(bool is_array);
	void open_array(const char* name,ObjectType* cur_type);

	void register_buildin_type();
};

class TreeRead
{
public:
	enum IT_CONVERSION
	{
		IT_LINEAR=0,//��� ������� ���� ���������������.
		IT_FIRST_READ=1,//������ ������ ������ ��������� � ��� �� �����, ����� �� ���������.
		IT_RANDOM_READ=2,//������� ���� �� � ��� �������, � ������� ��������.
	};
protected:
	typedef unsigned int POINTER_TYPE;
	struct ObjectType
	{
		string type_name;
		POINTER_TYPE size;

		TreeWrite::ObjectTypeIsPointer is_pointer:8;
		bool is_string;
		TreeBuildInType is_base:8;
		unsigned char user_type_data;

		struct Child
		{
			ObjectType* type;
			POINTER_TYPE offset;
			string name;
		};

		vector<Child> child;

		IT_CONVERSION conv;
		ObjectType():size(0),is_pointer(TreeWrite::OTIP_NOT_POINTER),is_string(false),
			is_base(TBIT_NONE),conv(IT_FIRST_READ),user_type_data(0){}
	};
public:
	class ObjectHandle
	{
		POINTER_TYPE begin_offset;
		ObjectType* type;
		friend TreeRead;
	public:
		ObjectHandle():begin_offset(-1),type(0){}
		ObjectHandle(POINTER_TYPE begin_offset_,TreeRead::ObjectType* type_):begin_offset(begin_offset_),type(type_){}

		bool is_empty()const{return type?true:false;}
		bool is_solid()const{return type->child.empty();}
		bool is_struct()const{return !type->child.empty();}
		bool is_pointer()const{return type->is_pointer?true:false;}
		bool is_array()const{return type->is_pointer==TreeWrite::OTIP_ARRAY;}
		bool is_string()const{return type->is_string;}

		bool is_null()const{return type==0;}
		TreeBuildInType is_base()const{return type->is_base;}//0 - �� ������� ���
		const char* type_name()const{return type->type_name.c_str();}

		//user_type_data �������������� ��� ������ ��������� ����� ����
		unsigned char& user_type_data(){return type->user_type_data;}
	};

	TreeRead(FileReadWriteFactory* writer=&default_read_write);
	~TreeRead();
	bool read(const char* file_name);

	ObjectHandle get_root(){return globals;};

	int get_num_object(const ObjectHandle& p);//������ ���� is_struct()
	ObjectHandle get_sub_object(const ObjectHandle& p,int i);
	const char* get_sub_object_name(const ObjectHandle& p,int i);


	ObjectHandle get_ptr(const ObjectHandle& p);//���� is_pointer()
	void get_string(const ObjectHandle& p,string& str);//���� is_string

	//���� is_array
	POINTER_TYPE get_array_size(const ObjectHandle& p);
	ObjectHandle get_array_elem(const ObjectHandle& p,POINTER_TYPE i);

	//���� is_solid
	bool get_bool(const ObjectHandle& p){return *(bool*)(data+p.begin_offset);}
	char get_char(const ObjectHandle& p){return *(char*)(data+p.begin_offset);}
	unsigned char get_uchar(const ObjectHandle& p){return *(unsigned char*)(data+p.begin_offset);}
	short get_short(const ObjectHandle& p){return *(short*)(data+p.begin_offset);}
	unsigned short get_ushort(const ObjectHandle& p){return *(unsigned short*)(data+p.begin_offset);}
	int get_int(const ObjectHandle& p){return *(int*)(data+p.begin_offset);}
	unsigned short get_uint(const ObjectHandle& p){return *(unsigned int*)(data+p.begin_offset);}
	float get_float(const ObjectHandle& p){return *(float*)(data+p.begin_offset);}
	double get_double(const ObjectHandle& p){return *(double*)(data+p.begin_offset);}

	void dump_all();
	
	//������� ��� �������� ������ �������/���� ���������� ������ � ���������
	//���� ������ �� ������, ���������� false
	//������������, ��� ��� ��������� ������ ��������� ����-�� ����, ������� �������� � ��������� ���-�� �������.
	//��� �����, ����� ���������� ������ � ������ ��� name.
	void begin_read_struct(const ObjectHandle& object,int& it);
	bool next_sub_object_by_name(const ObjectHandle& object,const char* name,ObjectHandle& sub_object,int& it);
	void end_read_struct(const ObjectHandle& object,int& it);
protected:
	FileReadWriteFactory* writer_;
	unsigned char* data;
	POINTER_TYPE data_size;

	ObjectHandle globals;


	vector<ObjectType> types;
	vector<ObjectType> buildin_types;
	void register_buildin_type();
	

	void dump_struct(FILE* f,ObjectHandle p,int recursion_num);
	void dump_tab(FILE* f,int recursion_num);

	ObjectType* get_type(POINTER_TYPE save_id)
	{
		POINTER_TYPE id=save_id&~(TreeWrite::FLAG_IS_BUILDIN);
		if(save_id&TreeWrite::FLAG_IS_BUILDIN)
			return &buildin_types[id];
		else
			return &types[id];
	}

};

#endif _TREEWRITE_H_
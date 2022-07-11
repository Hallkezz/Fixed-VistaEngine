//////////////////////////////////////////////////////////////////
//  ������� ��� ������� �������������
//
//  1. ����� ���������� � ������������.
//
//  2. �������: 
//	��������� �������
//	�������� �������
//	�������� true-������� � "�����������"
//	���������� � ������� ���������� �������
//
//  3. ����� �������� - stop().
//
//  4. ���� ������������� (����� SyncroTimer)
//	�� clock
//	�� frames - � ��������� ���������������� FPS
//
//////////////////////////////////////////////////////////////////
#ifndef	__DURATION_TIMER_H__
#define __DURATION_TIMER_H__

#include "SynchroTimer.h"

class Archive;

extern SyncroTimer global_time;	// ����������������� ��������������� ������
extern SyncroTimer frame_time; // ������������������� ����������������� ������
extern SyncroTimer scale_time; // ������������������� ���������� ������

//////////////////////////////////////////////////////////////////
//			������� ��� ����������������� ������
//////////////////////////////////////////////////////////////////
class BaseTimer {
public:
	BaseTimer() { time_ = 0; }
	void stop() { time_ = 0; }

	time_type get_start_time() const { return time_; }
	bool was_started() const { return (time_ > 0); }

	void serialize(Archive& ar);

protected:
	time_type time_;

	static const SyncroTimer& timer() { return global_time; }
};

// ��������� �������
class MeasurementTimer : public BaseTimer {
public:
	void start() { time_ = timer(); }	
	// ����� � ������� ������
	time_type operator () () const { return time_ ? timer() - time_ : 0; }
};

// ������ - ���������� � ������� ���������� �������

class DurationTimer : public BaseTimer {
public:
	void start(time_type duration) { time_ = timer() + duration; }	
	// true:  ��� start � �� ������ ����� duration, ���������� ������� �������
	time_type operator () () const { return time_ > timer() ? time_ - timer() : 0; }
	int operator ! () const { return (*this)() ? 0 : 1; } 
	void pause() { time_ += timer().delta(); } // �������� ������ ����� ��� ����������� �����
};

// ������ - �������� �������
class DelayTimer : public BaseTimer {
public:
	void start(time_type delay)	{ time_ = timer() + delay; }	
	// true:  ��� start � ������ ����� delay, ���������� ����� �� ����� ��������
	time_type operator () () const { return time_ && timer() >= time_ ? timer() - time_ + 1 : 0; }
	int operator ! () const { return (*this)() ? 0 : 1; } 
	void pause() { if(time_) time_ += timer().delta(); } // �������� ������ ����� ��� ����������� �����
};

// ������ - �������� true-������� 
class DelayConditionTimer : public BaseTimer {
public:
	int operator () (int condition, time_type delay);  // true: condition == true ����������� ����� delay �����. 
};

// ������ - ���������� true-������� 
class AverageConditionTimer : public BaseTimer {
public:
	int operator () (int condition, time_type delay);  // true: condition == true ����������� ����� delay. 
};

// ������ - ���������� true-������� 
class HysteresisConditionTimer : public BaseTimer {
	int turned_on;
public:
	HysteresisConditionTimer() { turned_on = 0; }
	// true: condition == true ����������� ����� on_delay, �����������, ���� condition == false ����� off_delay
	int operator () (int condition, time_type on_delay, time_type off_delay);  
	int operator () (int condition, time_type delay) { return (*this)(condition, delay, delay); }
	int operator () () { return turned_on; }
	void serialize(Archive& ar);
};

class InterpolationTimer : public MeasurementTimer {
public:
	InterpolationTimer() { durationInv_ = 0; }
	void start(time_type duration);
	float operator () () const; // [0..1]
	void serialize(Archive& ar);
private: 
	float durationInv_;
};

//////////////////////////////////////////////////////////////////
//			������� ��� �������
//////////////////////////////////////////////////////////////////
class BaseNonStopTimer {
public:
	BaseNonStopTimer() { time_ = 0; }
	void stop() { time_ = 0; }

	time_type get_start_time() const { return time_; }
	bool was_started() const { return time_ > 0; }

	void serialize(Archive& ar);

protected:
	time_type time_;

	static const SyncroTimer& timer() { return frame_time; }
};

// ��������� �������
class MeasurementNonStopTimer : public BaseNonStopTimer {
public:
	void start() { time_ = timer(); }	
	// ����� � ������� ������
	time_type operator () () const { return time_ ? timer() - time_ : 0; }
};

// ������ - ���������� � ������� ���������� �������

class DurationNonStopTimer : public BaseNonStopTimer {
public:
	void start(time_type duration) { time_ = timer() + duration; }	
	// true:  ��� start � �� ������ ����� duration, ���������� ������� �������
	time_type operator () () const { return time_ > timer() ? time_ - timer() : 0; }
	int operator ! () const { return (*this)() ? 0 : 1; } 
	void pause() { time_ += timer().delta(); } // �������� ������ ����� ��� ����������� �����
};

// ������ - �������� �������
class DelayNonStopTimer : public BaseNonStopTimer {
public:
	void start(time_type delay)	{ time_ = timer() + delay; }	
	// true:  ��� start � ������ ����� delay, ���������� ����� �� ����� ��������
	time_type operator () () const { return time_ && timer() >= time_ ? timer() - time_ + 1 : 0; }
	int operator ! () const { return (*this)() ? 0 : 1; } 
	void pause() { if(time_) time_ += timer().delta(); } // �������� ������ ����� ��� ����������� �����
};

class InterpolationNonStopTimer : public MeasurementNonStopTimer {
public:
	InterpolationNonStopTimer() { durationInv_ = 0; }
	void start(time_type duration);
	float operator () () const; // [0..1]
	void serialize(Archive& ar);
private: 
	float durationInv_;
};

#endif // __DURATION_TIMER_H__

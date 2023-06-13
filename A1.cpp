#include <iostream>
#include <vector>
#include <string>
using namespace std;

const char TIME_SEPERATOR = ':';
const string BETWEEN_TIMES = "-";
const char ENDING_CHARACTER = '#';

const int START_HOUR_CHAR = 0;
const int START_MINUTE_CHAR = 3;
const int END_HOUR_CHAR = 6;
const int END_MINUTE_CHAR = 9;

struct Time
{
	int hour;
	int minute;
};

struct Time_range
{
	Time start;
	Time end;
};
const Time_range DAY_RANGE = {{12, 0}, {20, 0}};

struct Work
{
	int needed_time;
	int free_range;
	Time_range planned_time;
};

bool has_ending_char(string input_string)
{
	bool return_value = false;
	if (input_string[0] == ENDING_CHARACTER)
	{
		return_value = true;
	}
	return return_value;
}

int calculate_input_time(string input_string, int starting_point)
{
	return (input_string[starting_point] - '0') * 10 + (input_string[starting_point + 1] - '0');
}

Time_range input_meeting(string input_string)
{
	Time_range meeting_time_range;
	meeting_time_range.start.hour = calculate_input_time(input_string, START_HOUR_CHAR);
	meeting_time_range.start.minute = calculate_input_time(input_string, START_MINUTE_CHAR);
	meeting_time_range.end.hour = calculate_input_time(input_string, END_HOUR_CHAR);
	meeting_time_range.end.minute = calculate_input_time(input_string, END_MINUTE_CHAR);
	return meeting_time_range;
}

Time add_minutes_to_time(Time start_time, int minutes_to_add)
{
	Time return_time;
	start_time.minute += minutes_to_add;
	return_time.minute = start_time.minute % 60;
	return_time.hour = start_time.hour + start_time.minute / 60;
	return return_time;
}

vector<Work> plan_works(vector<Work> works, vector<Time_range> free_times)
{
	for (int i = 0; i < works.size(); i++)
	{
		works[i].planned_time.start = free_times[works[i].free_range - 1].start;
		works[i].planned_time.end = add_minutes_to_time(works[i].planned_time.start, works[i].needed_time);
		free_times[works[i].free_range - 1].start = works[i].planned_time.end;
	}
	return works;
}

string time_int_to_string(int given_number)
{
	string output_string;
	output_string.push_back(given_number / 10 + '0');
	output_string.push_back(given_number % 10 + '0');
	return output_string;
}

string make_time_string(Time given_time)
{
	return (time_int_to_string(given_time.hour)) + TIME_SEPERATOR + (time_int_to_string(given_time.minute));
}

bool is_equal_time(Time time1, Time time2)
{
	return time1.hour == time2.hour && time1.minute == time2.minute;
}

vector<Time_range> read_meetings()
{
	vector<Time_range> meetings;
	string input_string;
	while (cin >> input_string)
	{
		if (has_ending_char(input_string))
			break;
		Time_range new_meeting = input_meeting(input_string);
		meetings.push_back(new_meeting);
	}
	return meetings;
}

vector<Work> read_works()
{
	vector<Work> works;
	int input_time, input_free_time;
	while (cin >> input_time)
	{
		cin >> input_free_time;
		Work new_work;
		new_work.needed_time = input_time;
		new_work.free_range = input_free_time;
		works.push_back(new_work);
	}
	return works;
}

vector<Time_range> find_free_times(vector<Time_range> meetings)
{
	vector<Time_range> free_times;
	Time_range new_free_time;
	if (meetings.size() == 0)
	{
		free_times.push_back(DAY_RANGE);
		return free_times;
	}
	if (!is_equal_time(meetings[0].start, DAY_RANGE.start))
		free_times.push_back({DAY_RANGE.start, meetings[0].start});
	for (int i = 0; i < meetings.size() - 1; i++)
	{
		new_free_time = {meetings[i].end, meetings[i + 1].start};
		if (!is_equal_time(new_free_time.start, new_free_time.end))
			free_times.push_back(new_free_time);
	}
	new_free_time = {meetings[meetings.size() - 1].end, DAY_RANGE.end};
	if (!is_equal_time(new_free_time.start, new_free_time.end))
		free_times.push_back(new_free_time);
	return free_times;
}

string generate_output(vector<Work> works)
{
	string output = "";
	for (int i = 0; i < works.size(); i++)
	{
		output += to_string(i + 1) + " ";
		output += make_time_string(works[i].planned_time.start) + BETWEEN_TIMES + make_time_string(works[i].planned_time.end);
		if (i != works.size() - 1)
			output += "\n";
	}
	return output;
}

int main()
{
	vector<Time_range> meetings = read_meetings();
	vector<Work> works = read_works();
	vector<Time_range> free_times = find_free_times(meetings);
	works = plan_works(works, free_times);
	string output = generate_output(works);
	cout << output;
	return 0;
}

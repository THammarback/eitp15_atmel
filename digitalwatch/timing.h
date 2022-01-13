#ifndef TIMING_H_
#define TIMING_H_

void clock_init();
int days_in_month(int month);
int time_get_weekday();

int time_get_seconds();
int time_get_minutes();
int time_get_hours();
int time_get_days();
int time_get_months();
int time_get_years();

void time_set_seconds(int s);
void time_set_minutes(int m);
void time_set_hours(int h);
void time_set_days(int d);
void time_set_months(int m);
void time_set_years(int y);



#endif /* TIMING_H_ */
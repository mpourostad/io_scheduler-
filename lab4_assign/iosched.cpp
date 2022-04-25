#include <cstdlib>
#include<sstream>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include<algorithm>
#include <iterator>
#include <typeinfo>
#include <queue>
#include <unistd.h>
#include <list>
#include <limits>
using namespace std;
int time_;
int track;
int tot_movement;

class io_request{
    public:
    int start_time;
    int start_track;
    int io_num;
    int disk_service_startt;
    int disk_service_endt;
    int wait_time;
    int max_wait_time;
    int turnaround;
    bool pending;
    io_request (int , int, int); 

};
io_request::io_request(int s, int t, int indx){
    start_time = s;
    start_track = t;
    io_num = indx;
    pending = false;
    // turnaround = 0;
    // wait_time = 0;
}
class Scheduler{
    public:
    queue <io_request*> io_queue;
    virtual io_request *fetch_next_request(){
        return nullptr;
    }
    virtual void add_to_queue(vector <io_request*> v){        
        ;
    }
};
class FIFO: public Scheduler{
    public:
    // queue <io_request*> io_queue;
    io_request *fetch_next_request(){
        io_request *p;
        if (io_queue.empty()){
            return nullptr;
        }
        else{
            p = io_queue.front();
            io_queue.pop();
            p ->pending = true;
            return p;
        }
    }
    void add_to_queue(vector <io_request*> v){
        for (int i = 0; i < v.size(); i++){
            if (v[i]->start_time == time_){
                // if (io_queue.empty()){
                //     io_queue.push(v[i]);
                //     break;
                // }
                // else if (io_queue.front()->io_num != v[i]->io_num){
                //     io_queue.push(v[i]);
                //     break;
                // }
                if (!v[i]->pending){
                    io_queue.push(v[i]);
                    break;
                }
            }
        }
    }
        
};
void print_io_request_info(vector <io_request*> req){
    for (int i = 0; i < req.size(); i++){
        printf("%5d: %5d %5d %5d\n",i, req[i]-> start_time, req[i]-> disk_service_startt, req[i]-> disk_service_endt);
    }
}
void print_io_stats(vector <io_request*> req){
    double wait_time_sum = 0;
    double turnaround_sum = 0;
    int max_waittime = 0;
    long sum_turn_around = 0;
    for (int i = 0; i < req.size(); i++){ 
        wait_time_sum += req[i]->disk_service_startt - req[i] -> start_time;
        turnaround_sum += req[i] ->disk_service_endt - req[i] -> start_time;
        if (req[i]->disk_service_startt - req[i] ->start_time > max_waittime){
            max_waittime = req[i]->disk_service_startt - req[i] -> start_time;
        }
    }
    double avg_turnaround = turnaround_sum / req.size();
    double avg_waittime = wait_time_sum / req.size();
    printf("SUM: %d %d %.2lf %.2lf %d\n",
        time_, tot_movement, avg_turnaround, avg_waittime, max_waittime);
}


int main(int argc, char** argv){
    // char *get_pager = NULL;
    tot_movement = 0;
    int num = 0;
    
    string filename_input = argv[1];
    vector <int> start_time;
    vector <int> track_num;
    vector <io_request*> io_request_p;
    ifstream f;
    string str;
    f.open(filename_input);
    while (getline(f, str))
    {
        int n = str.length();
        char stoc [n + 1];
        strcpy(stoc, str.c_str());
        int start;
        int track_;
        int success;
        // success = ;
        if (sscanf(stoc, "%d %d", &start, &track_) == 2){
            // start_time.push_back(start);
            // track_num.push_back(track_);
            io_request *ptr;
            ptr = new io_request(start, track_, num);
            num++;
            io_request_p.push_back(ptr);

        }

    }
    Scheduler *scheduler = new FIFO;
    // for (int i = 0; i < io_request_p.size(); i++){
    //     cout << io_request_p[i]->io_num << " " << io_request_p[i] ->start_time << " " << io_request_p[i] -> start_track << endl;
    // }
    time_ = 1;
    track = 0;
    bool complete = false;
    io_request *active_io = nullptr;
    while (true){        
        
        scheduler->add_to_queue(io_request_p);
        // cout << "that " << endl;
        // cout << scheduler -> io_queue.front();
        if (complete){
            active_io ->disk_service_endt = time_;
            // active_io -> turnaround = active_io ->disk_service_endt - active_io -> start_time;
            active_io = nullptr;
            complete = false;
            // time_++;
        }
        if (active_io == nullptr){            
            active_io = scheduler->fetch_next_request();
            // cout << active_io ->io_num << endl;
            if (active_io == nullptr){                
                break;
            }
            active_io-> disk_service_startt = time_;
            // cout << active_io ->io_num << ": " << active_io -> disk_service_startt << endl;
            // cout << scheduler -> io_queue.front();
            // continue;           
        }
        // else{
        if (track < active_io -> start_track){
            track++;
            tot_movement++;
            time_++;

        }
        else if (track > active_io -> start_track){
            track--;
            tot_movement++;
            time_++;  
        }
        if (track ==  active_io -> start_track){
            // active_io ->disk_service_endt = time_;
            complete = true;
            // cout << "that " << endl;
        }
        // }
              
    }
    print_io_request_info(io_request_p);
    print_io_stats(io_request_p);
        // if a new I/O arrived to the system at this current time
        // → add request to IO-queue
        // if an IO is active and completed at this time
        // → Compute relevant info and store in IO request for final summary if no IO request active now
        // if requests are pending
        // → Fetch the next request from IO-queue and start the new IO.
        // else if all IO from input file processed → exit simulation
        // if an IO is active
        // → Move the head by one unit in the direction its going (to simulate seek)
        // Increment time by 1
    return 0;
}
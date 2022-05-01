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
int direction;

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
    bool positive;
    io_request (int , int, int); 

};
io_request::io_request(int s, int t, int indx){
    start_time = s;
    start_track = t;
    io_num = indx;
    pending = false;
    positive = true;
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
    io_request *fetch_next_request(){
        io_request *p;
        if (io_queue.empty()){
            return nullptr;
        }
        else{
            p = io_queue.front();
            io_queue.pop();
            return p;
        }
    }
    void add_to_queue(vector <io_request*> v){
        for (int i = 0; i < v.size(); i++){
            if (v[i]->start_time == time_){
                if (!v[i]->pending){
                    io_queue.push(v[i]);
                    v[i] ->pending = true;
                    break;
                }
            }
        }
    }
        
};
class SSTF: public Scheduler{
    vector <io_request*> io_queue;
    io_request *fetch_next_request(){
        if (io_queue.empty()){
            return nullptr;
        }
        io_request *p;
        int shortest_seek = numeric_limits<int>::max();
        int index;
        
        for (int i = 0; i < io_queue.size(); i++){
            if (abs(track - io_queue[i] -> start_track) < shortest_seek){
                p = io_queue[i];
                index = i;
                shortest_seek = abs(track - io_queue[i] -> start_track);
            }
        }
        io_queue.erase(io_queue.begin() + index);
        // cout << "this" << endl;
        return p;


    }
    void add_to_queue(vector <io_request*> v){
        for (int i = 0; i < v.size(); i++){
            if (v[i]->start_time == time_){
                if (!v[i]->pending){
                    io_queue.push_back(v[i]);
                    v[i] ->pending = true;
                    break;
                }
            }
        }
    }
};
class Look: public Scheduler{
    vector <io_request*> io_queue;
    io_request *fetch_next_request(){
        if (io_queue.empty()){
            return nullptr;
        }
        io_request *p = nullptr;
        int shortest_seek = numeric_limits<int>::max();
        int index;
        
        for (int i = 0; i < io_queue.size(); i++){
            if (abs(track - io_queue[i] -> start_track) < shortest_seek){
                if ((track - io_queue[i] -> start_track) * direction <= 0){
                    p = io_queue[i];
                    index = i;
                    shortest_seek = abs(track - io_queue[i] -> start_track);
                }
                
            }
        }
        if (p == nullptr){
            for (int i = 0; i < io_queue.size(); i++){
                if (abs(track - io_queue[i] -> start_track) < shortest_seek){
                    p = io_queue[i];
                    index = i;
                    shortest_seek = abs(track - io_queue[i] -> start_track);
                
                }
            }
        }
        io_queue.erase(io_queue.begin() + index);
        // cout << "this" << endl;
        return p;
    }
    void add_to_queue(vector <io_request*> v){
        for (int i = 0; i < v.size(); i++){
            if (v[i]->start_time == time_){
                if (!v[i]->pending){
                    io_queue.push_back(v[i]);
                    v[i] ->pending = true;
                    break;
                }
            }
        }
    }
};
class CLook: public Scheduler{
    vector <io_request*> io_queue;
    io_request *fetch_next_request(){
        if (io_queue.empty()){
            return nullptr;
        }
        io_request *p = nullptr;
        int shortest_seek = numeric_limits<int>::max();
        int index;
        
        for (int i = 0; i < io_queue.size(); i++){
            if (abs(track - io_queue[i] -> start_track) < shortest_seek){
                if ((track - io_queue[i] -> start_track) <= 0){
                    p = io_queue[i];
                    index = i;
                    shortest_seek = abs(track - io_queue[i] -> start_track);
                }
                
            }
        }
        if (p == nullptr){
            int min_track_start = numeric_limits<int>::max();
            for (int i = 0; i < io_queue.size(); i++){
                if (io_queue[i] -> start_track < min_track_start){
                    p = io_queue[i];
                    index = i;
                    min_track_start = io_queue[i] -> start_track;
                
                }
            }
        }
        io_queue.erase(io_queue.begin() + index);
        // cout << "this" << endl;
        return p;
    }
    void add_to_queue(vector <io_request*> v){
        for (int i = 0; i < v.size(); i++){
            if (v[i]->start_time == time_){
                if (!v[i]->pending){
                    io_queue.push_back(v[i]);
                    v[i] ->pending = true;
                    break;
                }
            }
        }
    }
};
class FLook: public Scheduler{
    vector <io_request*> add_queue;
    vector  <io_request*> active_queue;
    io_request *fetch_next_request(){
        if (active_queue.empty()){
            if (add_queue.empty()){
                return nullptr;
            }
            else{
                active_queue.swap(add_queue);
            }         
        }
        io_request *p = nullptr;
        int shortest_seek = numeric_limits<int>::max();
        int index;
        
        for (int i = 0; i < active_queue.size(); i++){
            if (abs(track - active_queue[i] -> start_track) < shortest_seek){
                if ((track - active_queue[i] -> start_track) * direction <= 0){
                    p = active_queue[i];
                    index = i;
                    shortest_seek = abs(track - active_queue[i] -> start_track);
                }
                
            }
        }
        if (p == nullptr){
            for (int i = 0; i < active_queue.size(); i++){
                if (abs(track - active_queue[i] -> start_track) < shortest_seek){
                    p = active_queue[i];
                    index = i;
                    shortest_seek = abs(track - active_queue[i] -> start_track);
                
                }
            }
        }
        active_queue.erase(active_queue.begin() + index);
        return p;
    }
    void add_to_queue(vector <io_request*> v){
        for (int i = 0; i < v.size(); i++){
            if (v[i]->start_time == time_){
                if (!v[i]->pending){
                    add_queue.push_back(v[i]);
                    v[i] ->pending = true;
                    break;
                }
            }
        }
    }
};
bool is_complete(vector <io_request*> req){
    for (int i = 0; i < req.size(); i++){
        if (!req[i] -> pending){
            time_ = req[i] -> start_time;
            return false;

        }
    }
    return true;
}
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
    int c;
    char *strategy = NULL;
    strategy = optarg;
    Scheduler *scheduler;
    while ((c = getopt (argc, argv, "s:")) != -1){
        if (optarg[0] == 'i'){
            scheduler = new FIFO;
        }
        else if (optarg[0] == 'j'){
            scheduler = new SSTF;
        }
        else if (optarg[0] == 's'){
            scheduler = new Look;
        }
        else if (optarg[0] == 'c'){
            scheduler = new CLook;
        }
        else if (optarg[0] == 'f'){
            scheduler = new FLook;
        }
        else{
            cout <<" N/A " << endl;
        }
    }
    
    string filename_input = argv[2];
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
        if (sscanf(stoc, "%d %d", &start, &track_) == 2){
            io_request *ptr;
            ptr = new io_request(start, track_, num);
            num++;
            io_request_p.push_back(ptr);

        }
    }
    time_ = 0;
    track = 0;
    bool complete = false;
    direction = 1;
    io_request *active_io = nullptr;
    while (true){        
        
        scheduler->add_to_queue(io_request_p);
        if (complete){
            active_io ->disk_service_endt = time_;
            active_io = nullptr;
            complete = false;
        }
        if (active_io == nullptr){            
            active_io = scheduler->fetch_next_request();
            if (active_io == nullptr){   
                if (is_complete(io_request_p)){
                    break;
                }             
                continue;
            }
            active_io-> disk_service_startt = time_;        
        }
        // else{
        if (track < active_io -> start_track){
            direction = 1;
            track++;
            tot_movement++;
            time_++;

        }
        else if (track > active_io -> start_track){
            direction = -1;
            track--;
            tot_movement++;
            time_++;  
        }
        if (track ==  active_io -> start_track){
            complete = true;
        }
              
    }
    print_io_request_info(io_request_p);
    print_io_stats(io_request_p);
    return 0;
}
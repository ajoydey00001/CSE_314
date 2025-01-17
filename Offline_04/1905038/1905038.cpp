#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <semaphore.h>
#include <cmath>
#include <climits>
#include <vector>
#include <unistd.h>
#include <chrono>
#include <random>
#include <cstring>
#include<iostream>

#define Available 1
#define Busy 2
#define Ready 0
#define Terminate 1
#define Waiting 2
#define Running 3

using namespace std;

static std::random_device rd;
static std::mt19937 gen(rd());
std::poisson_distribution<int> poi_distribution(2.3);



int number_of_submission = 0;
int M, N, w, x, y;
vector<int> std_state;
vector<int> p_state;
vector<int> group_print_count;
vector<int> b_state;


sem_t mu_1,mu_2,mu_3,mu_4;
sem_t *stu_arr;
sem_t *stu_group;

sem_t empty_1;

struct my_read_write_lock
{
    sem_t read_lock;
    sem_t write_lock;
    int count;
} rw_lock;

void acquire_readlock(my_read_write_lock *object)
{
    sem_wait(&object->read_lock);
    object->count++;
    if (object->count == 1)
    {

        sem_wait(&object->write_lock);
    }
    sem_post(&object->read_lock);
}

void release_readlock(my_read_write_lock *object)
{
    sem_wait(&object->read_lock);
    object->count--;
    if (object->count == 0)
    {
        sem_post(&object->write_lock);
    }
    sem_post(&object->read_lock);
}

void acquire_writelock(my_read_write_lock *object)
{
    sem_wait(&object->write_lock);
}
void release_writelock(my_read_write_lock *object)
{
    sem_post(&object->write_lock);
}

auto processStartTime = std::chrono::system_clock::now();

inline double calcTime()
{
    return std::chrono::duration<double>(chrono::system_clock::now() - processStartTime).count();
}

inline int Random_Time_Interval()
{
    return poi_distribution(gen);
}

void test(int id, int station_id)
{
    if (p_state[station_id] == Available && std_state[id] == Waiting)
    {
        std_state[id] = Running;
        p_state[station_id] = Busy;
        sem_post(&stu_arr[id]);
    }
}

void acquire_print_station_1(int id, int station_id)
{
    sem_wait(&mu_1);
    std_state[id] = Waiting;
    test(id, station_id);
    sem_post(&mu_1);
    sem_wait(&stu_arr[id]);
    
}
void acquire_print_station_2(int id, int station_id)
{
    sem_wait(&mu_2);
    std_state[id] = Waiting;
    test(id, station_id);
    sem_post(&mu_2);
    sem_wait(&stu_arr[id]);
    
}
void acquire_print_station_3(int id, int station_id)
{
    sem_wait(&mu_3);
    std_state[id] = Waiting;
    test(id, station_id);
    sem_post(&mu_3);
    sem_wait(&stu_arr[id]);
    
}
void acquire_print_station_4(int id, int station_id)
{
    sem_wait(&mu_4);
    std_state[id] = Waiting;
    test(id, station_id);
    sem_post(&mu_4);
    sem_wait(&stu_arr[id]);
    
}

bool send_message_group_member(int id, int station_id, int gid)
{
    gid--;
    int i = gid * M;
    int flag = 0;
    for (int j = 1; j <= M; j++)
    {
        if (std_state[i + j] == Waiting && ((i + j) % 4 == station_id - 1) && p_state[station_id] == Available)
        {
            p_state[station_id] = Busy;
            sem_post(&stu_arr[i + j]);
            flag = 1;
           
        }
    }

    if (flag)
    {
        return true;
    }
    return false;
}
void send_message_other_group(int id, int station_id , int g_id)
{
    for (int i = 1; i <= N; i++)
    {
        int q = i/ M;
        int r = i % M;
         if (r != 0)
        {
            q++;
        }
        if (std_state[i] == Waiting && (i%4 == station_id -1)  && p_state[station_id] == Available && q != g_id)
        {
            p_state[station_id]  = Busy;
            sem_post(&stu_arr[i]);
           
        }
    }
}

void release_print_station_1(int id, int station_id)
{

    sem_wait(&mu_1);
    std_state[id] = Terminate;
    p_state[station_id] = Available;
    int q = id / M;
    int r = id % M;
    if (r != 0)
    {
        q++;
    }
    group_print_count[q]++;
    if (group_print_count[q] == M)
    {
        sem_post(&stu_group[q]);

        
    }
    if (!send_message_group_member(id, station_id, q))
    {
        send_message_other_group(id, station_id,q);
    }

    sem_post(&mu_1);
}

void release_print_station_2(int id, int station_id)
{

    sem_wait(&mu_2);
    std_state[id] = Terminate;
    p_state[station_id] = Available;
    int q = id / M;
    int r = id % M;
    if (r != 0)
    {
        q++;
    }
    group_print_count[q]++;
    if (group_print_count[q] == M)
    {
        sem_post(&stu_group[q]);

        
    }
    if (!send_message_group_member(id, station_id, q))
    {
        send_message_other_group(id, station_id,q);
    }

    sem_post(&mu_2);
}
void release_print_station_3(int id, int station_id)
{

    sem_wait(&mu_3);
    std_state[id] = Terminate;
    p_state[station_id] = Available;
    int q = id / M;
    int r = id % M;
    if (r != 0)
    {
        q++;
    }
    group_print_count[q]++;
    if (group_print_count[q] == M)
    {
        sem_post(&stu_group[q]);

        
    }
    if (!send_message_group_member(id, station_id, q))
    {
        send_message_other_group(id, station_id , q);
    }

    sem_post(&mu_3);
}
void release_print_station_4(int id, int station_id)
{

    sem_wait(&mu_4);
    std_state[id] = Terminate;
    p_state[station_id] = Available;
    int q = id / M;
    int r = id % M;
    if (r != 0)
    {
        q++;
    }
    group_print_count[q]++;
    if (group_print_count[q] == M)
    {
        sem_post(&stu_group[q]);

       
    }
    if (!send_message_group_member(id, station_id, q))
    {
        send_message_other_group(id, station_id , q);
    }

    sem_post(&mu_4);
}

void At_print_station(int pid)
{

    if (pid % 4 == 0)
    {

        acquire_print_station_1(pid, 1);
        printf("Student %d has started printing at time %.0lf \n", pid, calcTime());
        sleep(w);
        printf("------Student %d has finished printing at time %.0lf -------------\n", pid, calcTime());

        release_print_station_1(pid, 1);
    }
    else if (pid % 4 == 1)
    {
        acquire_print_station_2(pid, 2);
        printf("Student %d has started printing at time %.0lf\n", pid, calcTime());
        sleep(w);
        printf("------Student %d has finished printing at time %.0lf -----------\n", pid, calcTime());

        release_print_station_2(pid, 2);
    }
    else if (pid % 4 == 2)
    {
        acquire_print_station_3(pid, 3);
        printf("Student %d has started printing at time %.0lf\n", pid, calcTime());
        sleep(w);
        printf("------Student %d has finished printing at time %.0lf -------------\n", pid, calcTime());

        release_print_station_3(pid, 3);
    }
    else
    {

        acquire_print_station_4(pid, 4);
        printf("Student %d has started printing at time %.0lf\n", pid, calcTime());
        sleep(w);
        printf("------Student %d has finished printing at time %.0lf -------------\n", pid, calcTime());

        release_print_station_4(pid, 4);
    }
}

void At_bind_station(int gid)
{

    sem_wait(&empty_1);

    sleep(x);
    printf("Group %d has finished binding at time %.0lf\n", gid, calcTime());

    sem_post(&empty_1);
}

void go_to_Library(int gid)
{
    acquire_writelock(&rw_lock);
    sleep(y);
    number_of_submission++;
    printf("Group %d has submitted the report at time %.0lf\n", gid, calcTime());
    release_writelock(&rw_lock);
}

void *student_funtion(void *id)
{
    int pid = (int)((size_t)id);

    int interval = Random_Time_Interval();
    sleep(interval);

    printf("Student %d has arrived at the print station at time %.0lf\n", pid, calcTime());

    At_print_station(pid);

    if (pid % M != 0)
    {
        pthread_exit(NULL);
        return 0;
    }

    int g_id = pid / M;

    sem_wait(&stu_group[g_id]);

    printf("Group %d has finished printing at time %.0lf\n", g_id, calcTime());
    interval = Random_Time_Interval();
    sleep(interval);

    printf("Group %d  has started binding at time %.0lf\n", g_id, calcTime());

    At_bind_station(g_id);

    go_to_Library(g_id);

    pthread_exit(NULL);
    return 0;
}

void *staff_function(void *staff_id)
{
    int reader_id = (int)((size_t)staff_id);
    int number_of_group = (N/M);

    while (1)
    {
        int interval = Random_Time_Interval();
        sleep(interval);
        int flag = 0;
        acquire_readlock(&rw_lock);
        printf("Staff %d has started reading the entry book at time %.0lf . No. of submission = %d\n", reader_id, calcTime(), number_of_submission);
        sleep(y);
        if(number_of_submission == number_of_group){
            flag = 1;
        }
        release_readlock(&rw_lock);
        if(flag) break;
    }
    pthread_exit(NULL);
    return 0;
}

int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    

    srand(time(NULL));

    scanf("%d %d", &N, &M);
    scanf("%d %d %d", &w, &x, &y);

    
    sem_init(&mu_1,0,1);
    sem_init(&mu_2,0,1);
    sem_init(&mu_3,0,1);
    sem_init(&mu_4,0,1);
    sem_init(&empty_1, 0, 2);

    rw_lock.count = 0;
    sem_init(&rw_lock.read_lock, 0, 1);
    sem_init(&rw_lock.write_lock, 0, 1);

    for (int i = 0; i <= N; i++)
    {
        std_state.push_back(Ready);
    }
    for (int i = 0; i <= (N / M); i++)
    {
        group_print_count.push_back(0);
    }

    p_state.push_back(Available);
    p_state.push_back(Available);
    p_state.push_back(Available);
    p_state.push_back(Available);
    p_state.push_back(Available);

    b_state.push_back(Available);
    b_state.push_back(Available);
    b_state.push_back(Available);

    stu_arr = new sem_t[N + 1];
    int t = (N / M);
    stu_group = new sem_t[t + 1];
    for (int i = 0; i <= N; i++)
    {
        sem_init(&stu_arr[i], 0, 0);
    }
    for (int i = 0; i <= t; i++)
    {
        sem_init(&stu_group[i], 0, 0);
    }

    for (int i = 1; i <= N; i++)
    {
        pthread_t std;
      
        pthread_create(&std, NULL, student_funtion, (void *)((size_t)i));
    }

    pthread_t reader1, reader2;
    pthread_create(&reader1, NULL, staff_function, (void *)((size_t)1));
    pthread_create(&reader2, NULL, staff_function, (void *)((size_t)2));

    sem_destroy(&mu_1);
    sem_destroy(&mu_2);
    sem_destroy(&mu_3);
    sem_destroy(&mu_4);
    sem_destroy(&empty_1);
    sem_destroy(&rw_lock.read_lock);
    sem_destroy(&rw_lock.write_lock);

    for (int i = 0; i <= N; i++)
    {
        sem_destroy(&stu_arr[i]);
    }
    for (int i = 0; i <= t; i++)
    {
        sem_destroy(&stu_group[i]);
    }

    // fflush(stdout);
    pthread_exit(NULL);
    return 0;
}
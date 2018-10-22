/*
  Name: Olivier Ndikumana
  ID: 1001520973
*/


#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <semaphore.h>

/*** Constants that define parameters of the simulation ***/

#define MAX_SEATS 3        /* Number of seats in the professor's office */
#define professor_LIMIT 10 /* Number of students the professor can help before he needs a break */
#define MAX_STUDENTS 1000  /* Maximum number of students in the simulation */
#define MAX_CONSECUTIVE_STUDENTS 5 /* Number of consecutive students from each class limit */

#define CLASSA 0
#define CLASSB 1

static int students_in_office;   /* Total numbers of students currently in the office */
static int classa_inoffice;      /* Total numbers of students from class A currently in the office */
static int classb_inoffice;      /* Total numbers of students from class B in the office */
static int students_since_break; /* Total number of students helped since break */

// declaring semaphores that lock when there are three students in class
sem_t class_a;
sem_t class_b;

static int consecutive_students_a;
static int consecutive_students_b;


typedef struct
{
  int arrival_time;  // time between the arrival of this student and the previous student
  int question_time; // time the student needs to spend with the professor
  int student_id;
} student_info;

static int initialize(student_info *si, char *filename)
{
  students_in_office = 0;
  classa_inoffice = 0;
  classb_inoffice = 0;
  students_since_break = 0;

  // initializing semaphores
  sem_init(&class_a, 1, 1);
  sem_init(&class_b, 1, 1);

  /* Read in the data file and initialize the student array */
  FILE *fp;

  if((fp=fopen(filename, "r")) == NULL)
  {
    printf("Cannot open input file %s for reading.\n", filename);
    exit(1);
  }

  int i = 0;
  while ( (fscanf(fp, "%d%d\n", &(si[i].arrival_time), &(si[i].question_time))!=EOF) && i < MAX_STUDENTS )
  {
    i++;
  }

 fclose(fp);
 return i;
}

/* Code executed by professor to simulate taking a break
 * You do not need to add anything here.
 */
static void take_break()
{
  printf("The professor is taking a break now.\n...\n");
  sleep(5);
  assert( students_in_office == 0 );
  students_since_break = 0;
}

/* Code for the professor thread. This is fully implemented except for synchronization
 * with the students.  See the comments within the function for details.
 */
void *professorthread(void *junk)
{
  printf("The professor arrived and is starting his office hours\n");

  /* Loop while waiting for students to arrive. */
  while (1)
  {
    if (students_since_break == professor_LIMIT && students_in_office == 0)
      take_break();
  }
  pthread_exit(NULL);
}


void classa_enter()
{

  while (classb_inoffice > 0) {
  //  waits until there are no more class a students
  }

  // locks class as soon as a student enters
  sem_wait(&class_a);

  while(consecutive_students_a == MAX_CONSECUTIVE_STUDENTS || students_since_break == professor_LIMIT) {
    // waits when there have been 5 students or dude is no break
  }

  students_in_office += 1;
  students_since_break += 1;
  classa_inoffice += 1;
  consecutive_students_a += 1;

  //unlocks class if it's not full
  if (classa_inoffice != MAX_SEATS)
    sem_post(&class_a);
}

void classb_enter()
{

  while(classa_inoffice > 0) {
   // waits for class a to finish
  }

  // locks class as soon as a student enters
  sem_wait(&class_b);

  while(consecutive_students_b == MAX_CONSECUTIVE_STUDENTS || students_since_break == professor_LIMIT) {
    // waits when there have been 5 students or dude is no break
  }

  students_in_office += 1;
  students_since_break += 1;
  classb_inoffice += 1;
  consecutive_students_b += 1;

  //unlocks class if it's not full
  if (classb_inoffice != MAX_SEATS)
    sem_post(&class_b);


}

static void ask_questions(int t)
{
  sleep(t);
}


static void classa_leave()
{
  students_in_office -= 1;
  classa_inoffice -= 1;

  // sets number of consecutive students of a back to 0 after 5 students from b go through
  if (consecutive_students_b == MAX_CONSECUTIVE_STUDENTS && classa_inoffice == 0)
    consecutive_students_b = 0;

  // unlocks class if the student leaving was the third and the office was locked
  if (classa_inoffice == MAX_SEATS -1)
    sem_post(&class_a);

}

static void classb_leave()
{

   students_in_office -= 1;
   classb_inoffice -= 1;

   // sets number of consecutive students of a back to 0 after 5 students from a go through
   if (consecutive_students_a == MAX_CONSECUTIVE_STUDENTS && classb_inoffice == 0)
    consecutive_students_a = 0;

  // unlocks class if the student leaving was the third and the office was locked
  if (classb_inoffice == MAX_SEATS -1)
    sem_post(&class_b);
}


void* classa_student(void *si)
{
  student_info *s_info = (student_info*)si;

  /* enter office */
  classa_enter();

  printf("Student %d from class A enters the office\n", s_info->student_id);

  assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
  assert(classa_inoffice >= 0 && classa_inoffice <= MAX_SEATS);
  assert(classb_inoffice >= 0 && classb_inoffice <= MAX_SEATS);
  assert(classb_inoffice == 0 );

  /* ask questions  --- do not make changes to the 3 lines below*/
  printf("Student %d from class A starts asking questions for %d minutes\n", s_info->student_id, s_info->question_time);
  ask_questions(s_info->question_time);
  printf("Student %d from class A finishes asking questions and prepares to leave\n", s_info->student_id);

  /* leave office */
  classa_leave();

  printf("Student %d from class A leaves the office\n", s_info->student_id);

  assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
  assert(classb_inoffice >= 0 && classb_inoffice <= MAX_SEATS);
  assert(classa_inoffice >= 0 && classa_inoffice <= MAX_SEATS);

  pthread_exit(NULL);
}

void* classb_student(void *si)
{
  student_info *s_info = (student_info*)si;

  /* enter office */
  classb_enter();

  printf("Student %d from class B enters the office\n", s_info->student_id);

  assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
  assert(classb_inoffice >= 0 && classb_inoffice <= MAX_SEATS);
  assert(classa_inoffice >= 0 && classa_inoffice <= MAX_SEATS);
  assert(classa_inoffice == 0 );

  printf("Student %d from class B starts asking questions for %d minutes\n", s_info->student_id, s_info->question_time);
  ask_questions(s_info->question_time);
  printf("Student %d from class B finishes asking questions and prepares to leave\n", s_info->student_id);

  /* leave office */
  classb_leave();

  printf("Student %d from class B leaves the office\n", s_info->student_id);

  assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
  assert(classb_inoffice >= 0 && classb_inoffice <= MAX_SEATS);
  assert(classa_inoffice >= 0 && classa_inoffice <= MAX_SEATS);

  pthread_exit(NULL);
}

/* Main function sets up simulation and prints report
 * at the end.
 */
int main(int nargs, char **args)
{
  int i;
  int result;
  int student_type;
  int num_students;
  void *status;
  pthread_t professor_tid;
  pthread_t student_tid[MAX_STUDENTS];
  student_info s_info[MAX_STUDENTS];

  if (nargs != 2)
  {
    printf("Usage: officehour <name of inputfile>\n");
    return EINVAL;
  }

  num_students = initialize(s_info, args[1]);
  if (num_students > MAX_STUDENTS || num_students <= 0)
  {
    printf("Error:  Bad number of student threads. "
           "Maybe there was a problem with your input file?\n");
    return 1;
  }

  printf("Starting officehour simulation with %d students ...\n",
    num_students);

  result = pthread_create(&professor_tid, NULL, professorthread, NULL);

  if (result)
  {
    printf("officehour:  pthread_create failed for professor: %s\n", strerror(result));
    exit(1);
  }

  for (i=0; i < num_students; i++)
  {

    s_info[i].student_id = i;
    sleep(s_info[i].arrival_time);

    student_type = random() % 2;
    // student_type = CLASSB;

    if (student_type == CLASSA)
    {
      result = pthread_create(&student_tid[i], NULL, classa_student, (void *)&s_info[i]);
    }
    else // student_type == CLASSB
    {
      result = pthread_create(&student_tid[i], NULL, classb_student, (void *)&s_info[i]);
    }

    if (result)
    {
      printf("officehour: thread_fork failed for student %d: %s\n",
            i, strerror(result));
      exit(1);
    }
  }

  /* wait for all student threads to finish */
  for (i = 0; i < num_students; i++)
  {
    pthread_join(student_tid[i], &status);
  }

  /* tell the professor to finish. */
  pthread_cancel(professor_tid);

  printf("Office hour simulation done.\n");

  return 0;
}

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
#define MAX_CONSECUTIVE_STUDENTS 5

#define CLASSA 0
#define CLASSB 1

/* TODO */
/* Add your synchronization variables here */

/* Basic information about simulation.  They are printed/checked at the end
 * and in assert statements during execution.
 *
 * You are responsible for maintaining the integrity of these variables in the
 * code that you develop.
 */

static int students_in_office;   /* Total numbers of students currently in the office */
static int classa_inoffice;      /* Total numbers of students from class A currently in the office */
static int classb_inoffice;      /* Total numbers of students from class B in the office */
static int students_since_break;
static int number_of_breaks;

// pthread_mutex_t class_a;
// pthread_mutex_t class_b;
// pthread_mutex_t office_lock;

sem_t class_a;
sem_t class_b;
sem_t office_lock;

static int consecutive_students_a;
static int consecutive_students_b;


typedef struct
{
  int arrival_time;  // time between the arrival of this student and the previous student
  int question_time; // time the student needs to spend with the professor
  int student_id;
} student_info;

/* Called at beginning of simulation.
 * TODO: Create/initialize all synchronization
 * variables and other global variables that you add.
 */
static int initialize(student_info *si, char *filename)
{
  students_in_office = 0;
  classa_inoffice = 0;
  classb_inoffice = 0;
  students_since_break = 0;

  // initializing mutex
  // pthread_mutex_init(&class_a, NULL);
  // pthread_mutex_init(&class_b, NULL);
  // pthread_mutex_init(&office_lock, NULL);

  sem_init(&office_lock, 1, 1);
  sem_init(&class_a, 1, 1);
  sem_init(&class_b, 1, 1);

  /* Initialize your synchronization variables (and
   * other variables you might use) here
   */


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
  printf("The professor is back from break.\n");
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
    if (students_since_break == professor_LIMIT && students_in_office == 0) {
      take_break();
      number_of_breaks += 1;
    }

    // if (classa_inoffice > 0) {
    //   sem_wait(&class_b);
    //   printf("making b wait...\n");
    // }
    // if (classb_inoffice > 0) {
    //   sem_wait(&class_a);
    //   printf("making a wait...\n");
    // }

    // if (classa_inoffice == 0 && classb_inoffice == 0) {
    //   printf("b can now go in\n");
    //   sem_post(&class_b);
    // }
    //
    // if (classb_inoffice == 0 && classa_inoffice == 0) {
    //   printf("a can now go in\n");
    //   sem_post(&class_a);
    // }

    // if ( classa_inoffice == MAX_SEATS && pthread_mutex_trylock(&class_a) != 0 ) {
    //
    //   pthread_mutex_lock(&class_a);
    //   printf("locking class a because full\n");
    // }
    //
    // if ( classb_inoffice == MAX_SEATS && pthread_mutex_trylock(&class_b) != 0 ) {
    //
    //   pthread_mutex_lock(&class_b);
    //   printf("locking class b because full\n");
    // }
    //
    // if ( classa_inoffice > 0 && classa_inoffice < MAX_SEATS ) {
    //   sem_post(&class_a);
    //   // printf("unlocking class a because not full\n");
    // }
    // if ( classb_inoffice > 0 && classb_inoffice < MAX_SEATS ) {
    //
    //   sem_post(&class_b);
    //   // printf("unlocking class b because not full\n");
    // }
    //
    // //if there are no students in class a, meaning there is no lock, lock a students out and let class b students in
    // if ( classa_inoffice > 0 && pthread_mutex_trylock(&class_b) != 0 ) {
    //
    //   // pthread_mutex_unlock(&class_b);
    //   pthread_mutex_lock(&class_b);
    //   printf("locking b students out\n");
    // }
    // //
    // if ( classb_inoffice > 0 && pthread_mutex_trylock(&class_a) != 0 ) {
    //
    //   // pthread_mutex_unlock(&class_a);
    //   pthread_mutex_lock(&class_a);
    //   printf("locking a students out\n");
    // }
    //
    // if ( classa_inoffice == 0 && pthread_mutex_trylock(&class_a) == 0 ) {
    //
    //   pthread_mutex_unlock(&class_a);
    //   pthread_mutex_lock(&class_b);
    //   printf("No more students in class a. Letting class b students in\n");
    // }
    //
    // if ( classb_inoffice == 0 && pthread_mutex_trylock(&class_b) == 0 ) {
    //
    //   pthread_mutex_unlock(&class_b);
    //   pthread_mutex_lock(&class_a);
    //   printf("No more students in class b. Letting class a students in\n");
    // }

    // if (students_since_break == 0 && number_of_breaks != 0 && students_in_office == 0) {
    //   printf("%s\n", "done with break");
    //   pthread_mutex_unlock(&office_lock);
    // }


    // if (consecutive_students_a == MAX_CONSECUTIVE_STUDENTS && consecutive_students_a == MAX_CONSECUTIVE_STUDENTS) {
    //   consecutive_students_a = 0;
    //   consecutive_students_b = 0;
    // }

    /* TODO */
    /* Add code here to handle the student's request.             */
    /* Currently the body of the loop is empty. There's           */
    /* no communication between professor and students, i.e. all  */
    /* students are admitted without regard of the number         */
    /* of available seats, which class a student is in,           */
    /* and whether the professor needs a break. You need to add   */
    /* all of this.                                               */

  }
  pthread_exit(NULL);
}


/* Code executed by a class A student to enter the office.
 * You have to implement this.  Do not delete the assert() statements,
 * but feel free to add your own.
 */
void classa_enter()
{

  /* TODO */
  /* Request permission to enter the office.  You might also want to add  */
  /* synchronization for the simulations variables below                  */
  /*  YOUR CODE HERE.                                                     */

  //enters loop if classroom full
  // while (classb_inoffice != 0) {
  //   // waits until a student gets out
  // }

  // pthread_mutex_lock(&class_a);

  while (classb_inoffice > 0) {
  //  waits until a student gets out
  }

  sem_wait(&class_a);

  while(consecutive_students_a == MAX_CONSECUTIVE_STUDENTS || students_since_break == professor_LIMIT) {
    // waits when there have been 5 students or dude is no break
  }

  students_in_office += 1;
  students_since_break += 1;
  classa_inoffice += 1;
  consecutive_students_a += 1;

  printf("consecutive_students_a %d\n", consecutive_students_a);

  if (classa_inoffice != MAX_SEATS){
    printf("not full unlocking a...\n");
    sem_post(&class_a);
  }


  // printf("consecutive_students_a %d\n", consecutive_students_a);

}

/* Code executed by a class B student to enter the office.
 * You have to implement this.  Do not delete the assert() statements,
 * but feel free to add your own.
 */
void classb_enter()
{

  /* TODO */
  /* Request permission to enter the office.  You might also want to add  */
  /* synchronization for the simulations variables below                  */
  /*  YOUR CODE HERE.                                                     */

  while(classa_inoffice > 0) {
   // waits for class a to finish
  }

  sem_wait(&class_b);

  while(consecutive_students_b == MAX_CONSECUTIVE_STUDENTS || students_since_break == professor_LIMIT) {
    // waits when there have been 5 students or dude is no break
  }

  students_in_office += 1;
  students_since_break += 1;
  classb_inoffice += 1;
  consecutive_students_b += 1;

  printf("consecutive_students_b %d\n", consecutive_students_b);

  if (classb_inoffice != MAX_SEATS) {
    printf("not full unlocking b...\n");
    sem_post(&class_b);
  }


}

/* Code executed by a student to simulate the time he spends in the office asking questions
 * You do not need to add anything here.
 */
static void ask_questions(int t)
{
  sleep(t);
}


/* Code executed by a class A student when leaving the office.
 * You need to implement this.  Do not delete the assert() statements,
 * but feel free to add as many of your own as you like.
 */
static void classa_leave()
{
  /*
   *  TODO
   *  YOUR CODE HERE.
   */
  // last student to leave if there are still two more students unlocks
  students_in_office -= 1;
  classa_inoffice -= 1;

  if (consecutive_students_b == MAX_CONSECUTIVE_STUDENTS && classa_inoffice == 0)
    consecutive_students_b = 0;

  if (classa_inoffice == MAX_SEATS -1)
    sem_post(&class_a);

}

/* Code executed by a class B student when leaving the office.
 * You need to implement this.  Do not delete the assert() statements,
 * but feel free to add as many of your own as you like.
 */
static void classb_leave()
{
  /*
   * TODO
   * YOUR CODE HERE.
   */

   students_in_office -= 1;
   classb_inoffice -= 1;

   if (consecutive_students_a == MAX_CONSECUTIVE_STUDENTS && classb_inoffice == 0)
    consecutive_students_a = 0;

    if (classb_inoffice == MAX_SEATS -1)
      sem_post(&class_b);
}

/* Main code for class A student threads.
 * You do not need to change anything here, but you can add
 * debug statements to help you during development/debugging.
 */
void* classa_student(void *si)
{
  student_info *s_info = (student_info*)si;

  /* enter office */
  classa_enter();

  printf("Student %d from class A enters the office\n", s_info->student_id);

  printf("students_in_office %d\n", students_in_office);

  assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
  assert(classa_inoffice >= 0 && classa_inoffice <= MAX_SEATS);
  assert(classb_inoffice >= 0 && classb_inoffice <= MAX_SEATS);
  assert(classb_inoffice == 0 );

  printf("students_in_office %d\n", students_in_office);

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

/* Main code for class B student threads.
 * You do not need to change anything here, but you can add
 * debug statements to help you during development/debugging.
 */
void* classb_student(void *si)
{
  student_info *s_info = (student_info*)si;

  /* enter office */
  classb_enter();

  printf("Student %d from class B enters the office\n", s_info->student_id);

  printf("students_in_office %d\n", students_in_office);

  assert(students_in_office <= MAX_SEATS && students_in_office >= 0);
  assert(classb_inoffice >= 0 && classb_inoffice <= MAX_SEATS);
  assert(classa_inoffice >= 0 && classa_inoffice <= MAX_SEATS);
  assert(classa_inoffice == 0 );

  printf("students_in_office %d\n", students_in_office);

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

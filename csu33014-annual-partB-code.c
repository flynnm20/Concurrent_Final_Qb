//
// CSU33014 Summer 2020 Additional Assignment
// Part B of a two-part assignment
//
// Please write your solution in this file

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "csu33014-annual-partB-person.h"

void find_reachable_recursive(struct person *current, int steps_remaining,
                              bool *reachable)
{
  // mark current root person as reachable
  reachable[person_get_index(current)] = true;
  // now deal with this person's acquaintances
  if (steps_remaining > 0)
  {
    int num_known = person_get_num_known(current);
    for (int i = 0; i < num_known; i++)
    {
      struct person *acquaintance = person_get_acquaintance(current, i);
      find_reachable_recursive(acquaintance, steps_remaining - 1, reachable);
    }
  }
}

// computes the number of people within k degrees of the start person
int number_within_k_degrees(struct person *start, int total_people, int k)
{
  bool *reachable;
  int count;

  // maintain a boolean flag for each person indicating if they are visited
  reachable = malloc(sizeof(bool) * total_people);
  for (int i = 0; i < total_people; i++)
  {
    reachable[i] = false;
  }

  // now search for all people who are reachable with k steps
  find_reachable_recursive(start, k, reachable);

  // all visited people are marked reachable, so count them
  count = 0;
  for (int i = 0; i < total_people; i++)
  {
    if (reachable[i] == true)
    {
      count++;
    }
  }
  return count;
}

/* Q B 1: Explaination comments: Name: Matthew Flynn  Student Number :17327199
  In order to remove redunndency we need to remove the repeatly searching the same seen people. To do this I decided to programme a modified frontier seach.
  We start at the start person and create an We also create an array of booleans to keep track of which people have already been viewed. The array reached is 
  then initalised to false and afterwards the start person is initalised to true. We do this hear instead of within the frontier loop as it allows us to remove
  a redundant for loop within the find_reachable_recursive_less_redundant routine. We then create a frontier which we will be adding to as the process continues.
  The start person is added to the frontier.The main actions are preformed in the find reachable recursive less redundant function. This has been modiefied 
  from the find_reachable_recursive(). It now has 4 parameters the frontier, steps_remaining, reachable, frontiersize. For our first iteration the frontiersize 
  is given a 1 value. 
    find_reachable_recursive_less_redundant preformes one of 2 actions; creates a new frontier based on the acquaintances of the current frontier or it returns 
  as we have reached the k'th step away from the user. In the case where we haven't reached the end, the function creates a new frontier which it populates with 
  the acquaintances of the previous frontier which haven't been seen. I created a large frontier using malloc. The frontier can store up to 800000 people 
  pointers. This level frontier can handle huge populations as a result. Using the frontier size as the limit for the first for loop the program looks through the frontier
  To see how many acquaintances the person has. This number as the limit for the inner for loop. If it has been seen then it is passed over. If they haven't 
  been reached before then they are added to the frontier, are marked as reached and the size of the new frontier is updated so the whole frontier space isn't 
  iterated through every time.
    Once the new frontier has been created the old frontier memory is freed up to prevent usage of too much memory. Once that is done it enters 
  find_reachable_recursive_less_redundant with a new frontier, the steps remaining reduced by 1 and an updated reacable and frontier size. These changes mean that 
  there is no revisiting people already visited. Because of the frontier search there is a massive increase to the speed of the program. for small examples the
  speed can be almost 100 times faster and for larger ones, there is massive speedup however there is also  
*/
void find_reachable_recursive_less_redundant(struct person **frontier, int steps_remaining,
                                             bool *reachable, int frontiersize)
{
  // now deal with this person's acquaintances
  if (steps_remaining > 0) // haven't reached the desired level of seperation yet.
  {
    struct person **newfrontier = malloc(sizeof(struct person *) * 800000); // generate space for a new frontier.
    int newFrontierSize = 0;                                                // keep track of new frontier size.
    struct person *acquaintance;                                            // don't need to define this constantly
    for (int j = 0; j < frontiersize; j++)                                  // loop through all the elements in the frontier.
    {
      int num_known = person_get_num_known(frontier[j]); // get the number of acquaintances a person has
      for (int i = 0; i < num_known; i++)                // loop through all acquaintances
      {
        acquaintance = person_get_acquaintance(frontier[j], i); // get acquaintance pointer
        if (reachable[person_get_index(acquaintance)] == false) // found new person which hasn't been noted.
        {
          reachable[person_get_index(acquaintance)] = true; // mark new users as visited. done here to remove redundant loops at the beginning of the function.
          newfrontier[newFrontierSize] = acquaintance;      // add the new person to the new frontier
          newFrontierSize++;                                // update the size of the frontier to reflect the new added person.
        }
      }
    }
    free(frontier);                                                                                        // free the memory of the out of date frontier.
    find_reachable_recursive_less_redundant(newfrontier, steps_remaining - 1, reachable, newFrontierSize); // recurse until you have preformed the desired amount of iterations.
  }
  else
  {
    free(frontier); // have reached the end and just need to remove the last hanging frontier.
  }
}

// computes the number of people within k degrees of the start person;
// less repeated computation than the simple original version
int less_redundant_number_within_k_degrees(struct person *start,
                                           int total_people, int k)
{
  bool *reachable;
  int count;
  int size_of_frontier = 0;                                   // keep track of the size of the frontier.
  struct person **frontier = malloc(sizeof(struct person *)); // create a frontier.
  frontier[0] = start;
  // maintain a boolean flag for each person indicating if they are visited
  reachable = malloc(sizeof(bool) * total_people);
  for (int i = 0; i < total_people; i++)
  {
    reachable[i] = false;
  }
  reachable[person_get_index(start)] = true; // mark the start user as reached.
  count = 0;
  // now search for all people who are reachable with k steps
  find_reachable_recursive_less_redundant(frontier, k, reachable, 1);
  // all visited people are marked reachable, so count them
  for (int i = 0; i < total_people; i++)
  {
    if (reachable[i] == true)
    {
      count++;
    }
  }
  return count;
}

/* Q B 2: Explaination comments: Name: Matthew Flynn  Student Number :17327199
  When implementing the code in parallel there are many things to consider. I am using the frontier program I used in part 1 as it is efficient 
  and has very little dependencies across loops. There are 2 points where we can parallise the functions. 
    The first is the initalisation of the reachable bool array. There are no dependencies and it will speed up the initalisation. There is not
  much to really say about it. 
    The second parallel loop that is possible is when handling the acquaintance of the people within the frontier. This allows us to quickly move 
  through all the seen users and focus on the ones which haven't been used yet. Unfortuantly, there are some dependencies when adding them to the 
  new frontier. The new frontier variable must remain private for whoever is using it and thus so must the new frontier size and the acquaintance 
  being looked at. This means there is very little speedup within the for loop when dealing with users which need to be added to the frontier.
    The parrallisation of the system only really comes into effect when the number of people used is very high. I tested a 100 million and I saw.
  significat increases in the time it takes to run the program compared to my efficient version. This is due to the frontiers generally being small
  and thus most of the openmp stuff is simply creating overhead which isn't being outweighed by the current speed of the program. In generall I 
  found when running make time, the on an average run the parralised process took approximately 7% more time to complete then the efficient 
  version. When i ran for 100 million 10 then the parrallelised program was approximately twice as fast as the efficient version.
    In conclusion due to the construction of my optimized code, requiring few loops which are suitable for parralisation and when it is implemented
  it only increases overhead until the population of the graph is high enough. At which point the Openmp aspects stop contributing to overhead
  and make the system significantly faster then running it without them.
    
*/
int parallel_number_within_k_degrees(struct person *start,
                                     int total_people, int k)
{
  bool *reachable;
  int count;
  int size_of_frontier = 0;                                   // keep track of the size of the frontier.
  struct person **frontier = malloc(sizeof(struct person *)); // create a frontier.
  frontier[0] = start;
  // maintain a boolean flag for each person indicating if they are visited
  reachable = malloc(sizeof(bool) * total_people);
  // independent so can use parallel for loop
  //#pragma omp parallel for shared(reachable)
  for (int i = 0; i < total_people; i++)
  {
    reachable[i] = false;
  }

  reachable[person_get_index(start)] = true; // mark the start user as reached.
  count = 0;
  // now search for all people who are reachable with k steps
  find_reachable_recursive_less_redundant(frontier, k, reachable, 1);
  // all visited people are marked reachable, so count them
  //no dependencies so can use parallel for
  //#pragma omp parallel for
  for (int i = 0; i < total_people; i++)
  {
    if (reachable[i] == true)
    {
      count++;
    }
  }
  return count;
}

void find_reachable_recursive_parrallel(struct person **frontier, int steps_remaining,
                                        bool *reachable, int frontiersize)
{
  // now deal with this person's acquaintances
  if (steps_remaining > 0) // haven't reached the desired level of seperation yet.
  {
    struct person **newfrontier = malloc(sizeof(struct person *) * 500); // generate space for a new frontier.
    int newFrontierSize = 0;                                             // keep track of new frontier size.
    struct person *acquaintance;                                         // don't need to define this constantly
#pragma omp parallel private(i, j, acquaintance) shared(reachable, newfrontier, newFrontierSize)
    for (int j = 0; j < frontiersize; j++) // loop through all the elements in the frontier.
    {
      int num_known = person_get_num_known(frontier[j]); // get the number of acquaintances a person has
      for (int i = 0; i < num_known; i++)                // loop through all acquaintances
      {
        acquaintance = person_get_acquaintance(frontier[j], i); // get acquaintance pointer
        if (reachable[person_get_index(acquaintance)] == false) // found new person which hasn't been noted.
        {
          reachable[person_get_index(acquaintance)] = true; // mark new users as visited. done here to remove redundant loops at the beginning of the function.
          newfrontier[newFrontierSize] = acquaintance;      // add the new person to the new frontier
          newFrontierSize++;                                // update the size of the frontier to reflect the new added person.
        }
      }
    }
    free(frontier);                                                                                        // free the memory of the out of date frontier.
    find_reachable_recursive_less_redundant(newfrontier, steps_remaining - 1, reachable, newFrontierSize); // recurse until you have preformed the desired amount of iterations.
  }
  else
  {
    free(frontier); // have reached the end and just need to remove the last hanging frontier.
  }
}

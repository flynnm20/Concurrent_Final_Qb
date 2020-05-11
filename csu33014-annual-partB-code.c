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

/*
Student notes: Matthew Flynn Student No: 17327199
Part b 1:
The code produced will essentially search the graph like a search tree. Because of this we need to use an efficient search algorithm.
I am going to use a breathfirst search and implement it as a frontier search. As the system moves through people the acquaintances 
are added to the frontier so in order from lowest kernal to highest kernal. This will allow us to maintain a list of all pending 
people in our frontier and keep a list of our visited people in visisted people.
When an acquaintances is found it is checked if the person has already been visited. if so then the person is not interacted with further.
If it hasn't been found then it is added to the frontier and placed with relation to it's number of steps. 
Our frontier will be a FIFO stack which we will be added to by all aquantances being. The List of visited will also be a dynamic array. 
*/

void find_reachable_recursive(struct person **frontier, int steps_remaining,
                              bool *reachable, int frontiersize, int count)
{
  // now deal with this person's acquaintances
  if (steps_remaining > 0) // haven't reached the desired level of seperation yet.
  {
    struct person **newfrontier = malloc(sizeof(struct person *) * 500); // generate space for a new frontier.
    int newFrontierSize = 0;                                             // keep track of new frontier size.
    for (int j = 0; j < frontiersize; j++)                               // loop through all the eelements in the frontier.
    {
      int num_known = person_get_num_known(frontier[j]); // get the number of acquaintances a person has
      for (int i = 0; i < num_known; i++)                // loop through all acquaintances
      {
        struct person *acquaintance = person_get_acquaintance(frontier[j], i); // get acquaintance pointer
        if (reachable[person_get_index(acquaintance)] == false)                // found new person which hasn't been noted.
        {
          reachable[person_get_index(acquaintance)] = true; // mark new users as visited. done here to remove redundant loops at the beginning of the function.
          newfrontier[newFrontierSize] = acquaintance;      // add the new person to the new frontier
          count++;                                          // kep track of the number of people visited
          newFrontierSize++;                                // update the size of the frontier to reflect the new added person.
        }
      }
    }
    free(frontier);                                                                                // free the memory of the out of date frontier.
    find_reachable_recursive(newfrontier, steps_remaining - 1, reachable, newFrontierSize, count); // recurse until you have preformed the desired amount of iterations.
  }
  else
  {
    free(frontier); // have reached the end and just need to remove the last hanging frontier.
  }
}

// computes the number of people within k degrees of the start person
int number_within_k_degrees(struct person *start, int total_people, int k)
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
  count = 1;                                 // kep track of the people being reached to remove redundant for loop.
  // now search for all people who are reachable with k steps
  find_reachable_recursive(frontier, k, reachable, 1, count);
  return count;
}

// computes the number of people within k degrees of the start person;
// less repeated computation than the simple original version
int less_redundant_number_within_k_degrees(struct person *start,
                                           int total_people, int k)
{
  return number_within_k_degrees(start, total_people, k);
}

// computes the number of people within k degrees of the start person;
// parallel version of the code
int parallel_number_within_k_degrees(struct person *start,
                                     int total_people, int k)
{
  return number_within_k_degrees(start, total_people, k);
}

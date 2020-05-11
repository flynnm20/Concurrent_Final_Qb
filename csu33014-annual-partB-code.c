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
Our frontier will be a FIFO stack which we will be added to by all aquantances being . The List of visited will also be a dynamic array. 
*/

void find_reachable_recursive(struct person **frontier, int steps_remaining,
                              bool *reachable, int frontiersize)
{
  // deal with all users on the frontier
  for (int i = 0; i < frontiersize; i++)
  {
    reachable[person_get_index(frontier[i])] = true;
  }
  // mark current root person as reachable
  // now deal with this person's acquaintances
  if (steps_remaining > 0)
  {
    struct person **newfrontier;
    int newFrontierSize = 0; // keep track of new frontier size.
    for (int j = 0; j < frontiersize; j++)
    {
      struct person *current_person = frontier[j];
      int num_known = person_get_num_known(current_person);
      for (int i = 0; i < num_known; i++)
      {
        struct person *acquaintance = person_get_acquaintance(current_person, i);
        if (reachable[person_get_index(acquaintance)] == false) // found new member
        {
          realloc(frontier, sizeof(frontier) + sizeof(struct person)); // expand the frontier.
          newFrontierSize++;
          frontier[newFrontierSize - 1] = acquaintance;
        }
      }
    }
    free(frontier);
    find_reachable_recursive(newfrontier, steps_remaining - 1, reachable, newFrontierSize);
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
  // now search for all people who are reachable with k steps
  find_reachable_recursive(frontier, k, reachable, 1);
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

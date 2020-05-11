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
Our frontier will be a dynamic array which we can add to and sort. The List of visited will also be a dynamic array. 
*/

void find_reachable_recursive(struct person *frontier, int steps_remaining,
                              bool *reachable, struct person *visited)
{
  // mark current root person as reachable
  reachable[person_get_index(current)] = true;
  //add to visited.

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
  struct person *frontier = malloc(sizeof(struct person)); // create a frontier.
  frontier[0] = start;
  print(frontier[0]);
  struct person *visited = malloc(sizeof(struct person)); // create a list of visited people.
  // maintain a boolean flag for each person indicating if they are visited
  reachable = malloc(sizeof(bool) * total_people);
  for (int i = 0; i < total_people; i++)
  {
    reachable[i] = false;
  }

  // now search for all people who are reachable with k steps
  find_reachable_recursive(frontier, k, reachable, visited);

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

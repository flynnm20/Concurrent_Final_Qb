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

int find_reachable_recursive(struct person **frontier, int steps_remaining,
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
          count++;
          newFrontierSize++; // update the size of the frontier to reflect the new added person.
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
  return count;
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
  count = find_reachable_recursive(frontier, k, reachable, 1, count);
  // all visited people are marked reachable, so count them

  /*for (int i = 0; i < total_people; i++)
  {
    if (reachable[i] == true)
    {
      count++;
    }
  }*/
  return count;
}

// computes the number of people within k degrees of the start person;
// parallel version of the code
int parallel_number_within_k_degrees(struct person *start,
                                     int total_people, int k)
{
  return number_within_k_degrees(start, total_people, k);
}

// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-
//
// Copied from
// http://benchmarksgame.alioth.debian.org/u64q/program.php?test=binarytrees&lang=gpp&id=2
// and slightly modified (particularly by adding multi-threaded
// operation to hit malloc harder).
//
// This version of binary trees is mostly new/delete benchmark
//
// NOTE: copyright of this code is unclear, but we only distribute
// source.

/* The Computer Language Benchmarks Game
 * http://benchmarksgame.alioth.debian.org/
 *
 * Contributed by Jon Harrop
 * Modified by Alex Mizrahi
 * Adapted for gperftools and added threads by Aliaksei Kandratsenka
 */

#include <algorithm>
#include <errno.h>
#include <iostream>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>

// Modified version of binary_trees to find maximum page utilization.
// 31 July 2018

#include<map>
#include<iterator>

std::map<int,float> pageUtilization;
std::map<int,float> maxUtilization;

struct Node {
  Node *l, *r;
  int i;
  Node(int i2) : l(0), r(0), i(i2) {}
  Node(Node *l2, int i2, Node *r2) : l(l2), r(r2), i(i2) {}
  ~Node() { delete l; delete r; }
  int check() const {
    if (l) {
      return l->check() + i - r->check();
    } else {
      return i;
    }
  }
};

Node *make(int i, int d) {
  if (d == 0) return new Node(i);
  return new Node(make(2*i-1, d-1), i, make(2*i, d-1));
}
void run(int given_depth) {
  int min_depth = 4,
    max_depth = std::max(min_depth+2,
			 given_depth),
    stretch_depth = max_depth+1;

  {
    Node *c = make(0, stretch_depth);
    // *** Finding page utilization ***
		// Output statement
    // std::cout << "size of object: " << sizeof(*c) << std:: endl;
    // Shift right by 12 in order to get the page number
    
    int page_number = reinterpret_cast<uintptr_t>(c) >> 12;
    if (pageUtilization.find(page_number) != pageUtilization.end()){
         pageUtilization[page_number] += sizeof(*c)/4096.0;
    	 if (maxUtilization.find(page_number) != maxUtilization.end()){
      	   if (pageUtilization[page_number] > maxUtilization[page_number]){
        	  	maxUtilization[page_number] = pageUtilization[page_number];
        	 }            
  		 }
    	 else{
   		  	maxUtilization[page_number] = pageUtilization[page_number];
     	 }
			 std::cout << "value to be stored at address: " << c << "and page number: " << page_number << " is: " << pageUtilization[page_number] << std::endl;
		 }  
		 else{
        pageUtilization[page_number] = sizeof(*c)/4096.0;
        maxUtilization[page_number] = pageUtilization[page_number];
        std::cout << "value to be stored at page number: " << page_number << " is: " << sizeof(*c)/4096.0 << std::endl;
     }
  	// printf("allocated space of %d and address %p and page number is %d\n", static_cast<void*>(c), reinterpret_cast<uintptr_t>(*c) >> 12);
    std::cout << "stretch tree of depth " << stretch_depth << "\t "
      << "check: " << c->check() << std::endl;
    delete c;
	
	  // *** Finding page utilization ***
     std::cout << "size of object: " << sizeof(*c) << std:: endl; 
     int page_number2 = reinterpret_cast<uintptr_t>(c) >> 12;
     if (pageUtilization.find(page_number2) != pageUtilization.end()){
         pageUtilization[page_number2] -= sizeof(*c)/4096.0;
    	 if (maxUtilization.find(page_number2) != maxUtilization.end()){
      	   if (pageUtilization[page_number2] > maxUtilization[page_number2]){
        	  	maxUtilization[page_number2] = pageUtilization[page_number2];
        	 }            
  		 }
    	 else{
   		  	maxUtilization[page_number2] = pageUtilization[page_number2];
    	 }
  		 std::cout << "2value to be deleted from address: " << c << "and page number: " << page_number2 << " is: " << pageUtilization[page_number2] << std::endl;
 		 } 
 		 else{
        pageUtilization[page_number2] = sizeof(*c)/4096.0;
        maxUtilization[page_number2] = pageUtilization[page_number2];
        std::cout << "value to be stored at page number: " << page_number2 << " is: " << sizeof(*c)/4096.0 << std::endl;
      }
  	 //printf("allocated space of %d and address %p and page number is %d\n", static_cast<void*>(c), reinterpret_cast<uintptr_t>(*c) >> 12);

  }

  Node *long_lived_tree=make(0, max_depth);

  for (int d=min_depth; d<=max_depth; d+=2) {
    int iterations = 1 << (max_depth - d + min_depth), c=0;
    for (int i=1; i<=iterations; ++i) {
      Node *a = make(i, d), *b = make(-i, d);

     // *** Finding page utilization ***
     //std::cout << "size of object: " << sizeof(*a) << std:: endl;
     int page_number3 = reinterpret_cast<uintptr_t>(a) >> 12;
     if (pageUtilization.find(page_number3) != pageUtilization.end()){
         pageUtilization[page_number3] += sizeof(*a)/4096.0;
    	 if (maxUtilization.find(page_number3) != maxUtilization.end()){
      	   if (pageUtilization[page_number3] > maxUtilization[page_number3]){
        	  	maxUtilization[page_number3] = pageUtilization[page_number3];
        	 }            
  		 }
    	 else{
   		  	maxUtilization[page_number3] = pageUtilization[page_number3];
    	 }
    	 std::cout << "3value to be stored at address: " << a << "and page number: " << page_number3 << "is: " << sizeof(*a)/4096.0 << std::endl;
		 }  
		 else{
        pageUtilization[page_number3] = sizeof(*a)/4096.0;
        maxUtilization[page_number3] = pageUtilization[page_number3];
        std::cout << "value to be stored at page number: " << page_number3 << "is: " << sizeof(*a)/4096.0 << std::endl;
      }
    
     // *** Finding page utilization ***
     //std::cout << "size of object: " << sizeof(*b) << std:: endl;
     int page_number4 = reinterpret_cast<uintptr_t>(b) >> 12;
     if (pageUtilization.find(page_number4) != pageUtilization.end()){
         pageUtilization[page_number4] += sizeof(*b)/4096.0;
    	 if (maxUtilization.find(page_number4) != maxUtilization.end()){
      	   if (pageUtilization[page_number4] > maxUtilization[page_number4]){
        	  	maxUtilization[page_number4] = pageUtilization[page_number4];
        	 }            
  		 }
    	 else{
   		  	maxUtilization[page_number4] = pageUtilization[page_number4];
    	 }
    	 std::cout << "4value to be stored at address: " << b << "and page number: " << page_number4 << "is: " << sizeof(*b)/4096.0 << std::endl;
 			}  
 			else{
        pageUtilization[page_number4] = sizeof(*b)/4096.0;
        maxUtilization[page_number4] = pageUtilization[page_number4];
        std::cout << "value to be stored at page number: " << page_number4 << "is:" << sizeof(*b)/4096.0 << std::endl;
       }
    c += a->check() + b->check();
      delete a;
      delete b;
   
     // *** Finding page utilization ***
     //std::cout << "size of object: " << sizeof(*a) << std:: endl;
     int page_number5 = reinterpret_cast<uintptr_t>(a) >> 12;
     if (pageUtilization.find(page_number5) != pageUtilization.end()){
   				pageUtilization[page_number5] -= sizeof(*a)/4096.0;
    	 if (maxUtilization.find(page_number5) != maxUtilization.end()){
      	   if (pageUtilization[page_number5] > maxUtilization[page_number5]){
        	  	maxUtilization[page_number5] = pageUtilization[page_number5];
        	 }            
  		 }
	  	 else{
   		  	maxUtilization[page_number5] = pageUtilization[page_number5];
  		 }
    	 std::cout << "5value to be deleted from address: " << a << "and page number: " << page_number5 << "is : " << sizeof(*a)/4096.0 << std::endl;
 
		  }  
		  else{
        pageUtilization[page_number5] = sizeof(*a)/4096.0;
        maxUtilization[page_number5] = pageUtilization[page_number5];
        std::cout << "value to be stored at page number: " << page_number5 << "is: " << sizeof(*a)/4096.0 << std::endl;
      } 

     // *** Finding page utilization ***
     //std::cout << "size of object: " << sizeof(*b) << std:: endl;
     int page_number6 = reinterpret_cast<uintptr_t>(b) >> 12;
     if (pageUtilization.find(page_number6) != pageUtilization.end()){
         pageUtilization[page_number6] -= sizeof(*b)/4096.0;
    	 if (maxUtilization.find(page_number6) != maxUtilization.end()){
      	   if (pageUtilization[page_number6] > maxUtilization[page_number6]){
        	  	maxUtilization[page_number6] = pageUtilization[page_number6];
        	 }            
  		 }
    	 else{
   		  	maxUtilization[page_number6] = pageUtilization[page_number6];
    	 }
    	 std::cout << "6value to be deleted from address: " << b << "and page number: " << page_number6 << "is: " << sizeof(*b)/4096.0 << std::endl;
 			} 
 			else{
        pageUtilization[page_number6] = sizeof(*b)/4096.0;
        maxUtilization[page_number6] = pageUtilization[page_number6];
        std::cout << "value to be stored at page number: " << page_number6 << "is :" << sizeof(*b)/4096.0 << std::endl;
       }
   }
    std::cout << (2*iterations) << "\t trees of depth " << d << "\t "
	      << "check: " << c << std::endl;
  }

  std::cout << "long lived tree of depth " << max_depth << "\t "
	    << "check: " << (long_lived_tree->check()) << "\n";

  delete long_lived_tree;
}

static void *run_tramp(void *_a) {
  intptr_t a = reinterpret_cast<intptr_t>(_a);
  run(a);
  return 0;
}

int main(int argc, char *argv[]) {
  int given_depth = argc >= 2 ? atoi(argv[1]) : 20;
  int thread_count = std::max(1, argc >= 3 ? atoi(argv[2]) : 1) - 1;
  std::vector<pthread_t> threads(thread_count);

  for (int i = 0; i < thread_count; i++) {
    int rv = pthread_create(&threads[i], NULL,
                            run_tramp,
                            reinterpret_cast<void *>(given_depth));
    if (rv) {
      errno = rv;
      perror("pthread_create");
    }
  }
  run_tramp(reinterpret_cast<void *>(given_depth));
  for (int i = 0; i < thread_count; i++) {
    pthread_join(threads[i], NULL);
  }
 
 //print final page utilization map
 // float counter = 0;
  printf("Page Number    Max Utilization\n");
  std::map<int,float>::iterator it = maxUtilization.begin();
  while (it != maxUtilization.end()) {
  std::cout<<it->first<<"     " << it->second<<std::endl;
  it++;
 // counter+= maxUtilization[it];
  }
 // float avgMax = counter/sizeof(maxUtilization);
  return 0;
}

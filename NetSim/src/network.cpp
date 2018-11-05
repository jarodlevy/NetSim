#include "network.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <functional>
#include "random.h"

/*! Resizes the list of nodes (\ref values)
 After this function is called \ref values has size *n* and contains random numbers (normal distribution, mean=0, sd=1).
*/

void Network::resize(const size_t& resize) {

	values.resize(resize);  // Use of the resize function of the vector class 
    RNG.normal(values);
}

/*! Adds a bidirectional link between two nodes
  @param[in] a,b the indexes if the two nodes 
  @param[out] success the link was succesfully inserted
 */
 
bool Network::link_existence(const size_t& a,const size_t& b) {

 	for (auto i: links) {
 		if((i.first == a or i.first == b) and (i.second == b or i.second == a)) { 
 			return true;
 		}
 	}

 	return false;
}
 
bool Network::add_link(const size_t& a, const size_t& b) {

		if ((a!=b) and 	// a and b have to be different to create a link
           (std::max(a,b) < values.size()) and // we check that the value of a and b are in the vector
		   (!link_existence(a,b))) { // we check in an other function if the link exist or not
		   
           links.insert(std::pair<size_t, size_t>(a,b));  // We insert twice to have the bidirectionnal link
		   links.insert(std::pair<size_t,size_t>(b,a));
		   return true;
		}
	
	return false;
}

/*! Creates random connections between nodes: each node *n* will be linked with *degree(n)* other nodes (randomly chosen) where *degree(n)* is Poisson-distributed.

  All previous links are cleared first.
  @param mean_deg the average of the Poisson distribution.
 */
 
size_t Network::random_connect(const double& mean_deg) {

	links.clear(); // We clear the map before fill it with other values
	
	for(size_t i(0); i < values.size(); ++i) {

        size_t deg(RNG.poisson(mean_deg));

		for(size_t j(0); j < deg; ++j) {
            while(!add_link(i,RNG.uniform_double(0, values.size()))) continue; // This command was hard to find, while we can't add a link we change values until it works
		}
	}
 return links.size()/2; // All links are bidirectional so we divide the numbers of links by 2
}

/*! Resets all node values.
  @param[in] vector of new node values
  @param[out] number of nodes succesfully reset
 */
size_t Network::set_values(const std::vector<double>& new_values) {
	values.clear();
	values = new_values;
	
	return values.size();
}

/*! Number of nodes */

size_t Network::size() const {

	return values.size();
}

/*! Degree (number of links) of node no *n* */

size_t Network::degree(const size_t & n) const {

	return links.count(n); // We use the useful multimap function count which searches the elements with the same key
}

/*! Value of node no *n* */

double Network::value(const size_t & n) const {
	if(n < values.size()) return values[n];
	
    return 0;

			
}

/*! All node values in descending order */

std::vector<double> Network::sorted_values() const {

	std::vector<double> result(values);

	std::sort (result.begin(), result.end(), std::greater<double>()); // To code this function, we look at the test you've made 

	return result;
}

/*! All neighbors (linked) nodes of node no *n* */

std::vector<size_t> Network::neighbors(const size_t& n) const {

    std::vector<size_t> neighbors;

	for (auto i : links) {
		if(i.first == n) {
            neighbors.push_back(i.second);
		}
	}

    return neighbors;
}


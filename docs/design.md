## Initial Design (v0.1.0)
The initial design of the matching engine is the naive approach. My design philosophy was to not worry
the efficiency in any way but instead, the easiest method was implemented first. This was done to be
a benchmark for a naive approach to the solution.

I have also decided to only implement limit orders with add order as the only order type. I will later
try and implement cancel and modify orders. There will however need to be other data structures in place
for this as i will need to keep track of orders that have been filled or partially filled. This is
because there are many cases where someone tries to cancel an order that has already been filled.

The next implementation that I will be implementing is a refactor of the engine to be more efficient
and to increase readability.

## OrderPointer / OrderPointers Data Structure (v0.2.0)
Moving to std::shared_ptr for Orders was don't to reduce the amount of data copying that was happening,
this at the moment does not have that large of an effect on performance, only creating a 15.8% increase
in speed to match an order, really only significantly reducing the amount of ram being used in large
data sets. 

This was however not the intention on why I moved to this data structure, the reason that I moved to
this data structure was that I will be implementing both Cancel and Modify order types to the OrderBook.
To be able to efficiently look-up an order by its order ID within the orderbook, I decided to implement
an unordered map that contains both a pointer to the order and the iterator of its position within the
orders vector of its price level.

This allows for a O(1) lookup time for an order, thus, dramatically decreasing the lookup time from the
original, but unimplemented (only found as a commented out block) CancelOrder() method that had a
time complexity of O(n^2) which was horrifically inefficient and would subsequently cause datasets of
only 1.2GB to run for upwards of 10 minutes during early testing. 

This is what initially contributed to CancelOrder and ModifyOrder being left unimplemented for v0.1.0.

Regardless, this improvement to the underlying data structure of the program is laying the foundation
for the rest of the Matching engine.
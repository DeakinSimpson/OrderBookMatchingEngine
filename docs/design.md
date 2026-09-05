## Initial Design (v0.1.0)
The initial design of the matching engine is the naive approach. My design philosophy was to not worry
the efficiency in any way but instead, the easiest method was implemented first. This was done to be
a benchmark for a naive approach to the solution.

I have also decided to only implement limit orders with add order as the only order type. I will later
try and implement cancel and modify orders. There will however need to be other data structures in place
for this as i will need to keep track of orders that have been filled or partially filled. This is
because there are many cases where someone tries to cancel an order that has already been filled.

The next implementation that I will be implementing is a refactor of the engine to be more efficient
and to increase readibility.
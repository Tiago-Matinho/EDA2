# EDA2-Airport

**Djikstra's** algorithm implementation using hashtables and a binary heap.

Due to the short number of ***direct flights*** between two airports, sometimes when flying, we have to take ***multiple*** flights to reach our destination.
So when booking a trip we have to take into account not only the flight schedule but also the **duration*** of the flights as well as the ***time spent in airports*** waiting for the next flight.

The task is to calculate the ***shortest trip*** from one airport to another, and show all the flights needed to reach the final destination, taking into account local ***timezones***.

This results in a ***complex graph*** as there can be multiple flights with the same route but leaving at ***different*** schedules.

# Fastest Flight Connection

**Dijkstra's** algorithm implementation using *hashtables* and a *binary heap*.

Due to the short number of **direct flights** between two airports, sometimes when flying, we have to take **multiple** flights to reach our destination.<br>
So when booking a trip we have to take into account not only the flight schedule but also the **duration** of the flights as well as the **time spent in airports** waiting for the next flight.

The task is to calculate the **shortest trip** from one airport to another and show all the flights needed to reach the final destination, taking into account local **timezones**.

This results in a **complex graph** as there can be multiple flights with the same route but leaving at **different** schedules.

This program must **support** the following operations:
- Add a new airport;
- Add a new flight schedule;
- Remove a flight schedule;
- Calculate the shortest trip.

## Constraints
All data should be **stored** locally and must be able to store 200000 airports and 750000 flights.<br>
The program's **memory** use is limited to 64MB (RAM) and 2GB (Disk).<br>
The program is also limited by **time** and should be able to run all the inputs in under 3 seconds.

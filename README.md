# Delivery Robot Hamiltonian Circuit Problem

## Problem Description

Imagine you are tasked with designing a delivery robot that operates within a warehouse. The robot's mission is to:

1. Start at a designated "home" location.
2. Visit every other location exactly once.
3. Return to the home location before its battery runs out.

The warehouse layout is represented as an **undirected graph G = (V, E)**:

- Each vertex (v in V) represents a location.
- Each edge ((u, v) in E) represents a direct path between locations (u) and (v).

### Additional Details:

- (h in V) is the designated home vertex.
- Each location (v in V) (other than (h)) has a delivery time (t(v)).
- The robot must complete its route within a time limit (T), which includes delivery and travel times.

### Objective:

Find a **Hamiltonian circuit** in (G) that:

- Starts and ends at (h).
- Visits every other vertex exactly once.
- Ensures the total time does not exceed (T).

If such a circuit exists, output the sequence of vertices visited. If not, output `NO FEASIBLE CIRCUIT`.

---

## Solution Approach

### Algorithm Design

We use a recursive backtracking approach combined with a **priority queue** for optimal neighbor selection. Below is the pseudocode:

1. Initialize variables:

   ```
   cost = 0
   path = []
   visited[V] = [false, false, ..., false]
   visited[h] = true  // h is the home vertex
   path.add(h)  // Start the path with the home vertex
   call hamiltonCycle(graph, visited, h, path, cost)
   ```

2. Function Definition:

   ```
   bool hamiltonCycle(graph, visited, h, path, cost):
       if path.size() == V:
           // Check if the last visited vertex connects back to h
           if (last_vertex in path has edge to h):
               display path
               return true
           else:
               return false

       found = false
       priority_queue pq
       for each neighbor of current vertex:
           if not visited[neighbor]:
               pq.push(neighbor, weight + graph.TravelCost(current, neighbor))

       while not pq.empty() and not found:
           neighbor = pq.top()
           pq.pop()

           visited[neighbor] = true
           path.add(neighbor)
           cost += (delivery_time + travel_time)

           found = hamiltonCycle(graph, visited, h, path, cost)

           visited[neighbor] = false
           cost -= (delivery_time + travel_time)
           path.remove()

       return found
   ```

### Complexity Analysis

#### **Time Complexity:**

1. **Best Case:**

   - Priority queue operations: O(V · log V)
   - Depth of recursion: O(V)
   - Overall: O(V² · log V)

2. **Worst Case:**
   - Exploring all permutations: O(V!)
   - Combined complexity: O(V · log V + V!)

#### **Space Complexity:**

1. Graph representation: O(V²)
2. Auxiliary space (visited array, path vector, priority queue): O(V²)
3. Overall: O(V²)

---

## Input Format

1. **Graph Definition:**
   - (V): Set of vertices.
   - (E): Set of edges with weights.
2. **Delivery Times:**
   - (t(v)): Delivery time for each vertex.
3. **Time Limit:**
   - (T): Maximum allowable time.

### Example Test Cases

#### Test Case 1:

- **Vertices:** V = {h, A, B, C}
- **Edges:** E = {(h, A), (h, B), (h, C), (A, B), (A, C), (B, C)}
- **Weights:** [2, 2, 3, 4, 5, 8]
- **Delivery Times:** t(A) = 5, t(B) = 10, t(C) = 8
- **Time Limit:** T = 38
- **Expected Output:** (h, B, A, C, h)

#### Test Case 2:

- **Vertices:** V = {h, A, B, C}
- **Edges:** E = {(h, A), (h, B), (h, C), (A, B), (A, C), (B, C)}
- **Weights:** [1, 2, 3, 4, 5, 6]
- **Delivery Times:** t(A) = 5, t(B) = 10, t(C) = 8
- **Time Limit:** T = 20
- **Expected Output:** NO FEASIBLE CIRCUIT

---

## Program Implementation

The solution has been implemented in C++ and named `hcp.cpp`. It processes multiple test cases from input text files.

### Steps:

1. Read and parse input files.
2. Execute the Hamiltonian Circuit algorithm for each test case.
3. Output results for each test case.

---

## Algorithm Diagram

![Basic_Algortihm](https://github.com/user-attachments/assets/8a0ef554-6a70-4388-8d24-4fe3687f72c6)

---

import sys

# Define the total number of possible characters (26 lowercase English letters)
ALPHABET_SIZE = 26

# Function to convert a character to its index
def char_to_index(c):
    return ord(c) - ord('a')

# Function to convert an index to its corresponding character
def index_to_char(i):
    return chr(i + ord('a'))

# Floyd-Warshall algorithm to compute shortest paths between all pairs of nodes
def floyd_warshall(graph):
    # Initialize the graph where graph[i][j] is the shortest time from node i to j
    dist = [[float('inf')] * ALPHABET_SIZE for _ in range(ALPHABET_SIZE)]
    
    # Initialize the direct connections from the graph
    for u in range(ALPHABET_SIZE):
        dist[u][u] = 0  # No time required to convert a character to itself
    
    for u in range(ALPHABET_SIZE):
        for v, time in graph[u]:
            dist[u][v] = min(dist[u][v], time)  # Minimum time for direct conversion
    
    # Apply Floyd-Warshall to compute the shortest paths between all pairs of nodes
    for k in range(ALPHABET_SIZE):
        for i in range(ALPHABET_SIZE):
            for j in range(ALPHABET_SIZE):
                if dist[i][j] > dist[i][k] + dist[k][j]:
                    dist[i][j] = dist[i][k] + dist[k][j]
    
    return dist

def minimum_conversion_time(strPwd, strInter, firstSeq, secSeq, timeSeq):
    # Initialize the graph with adjacency list representation
    graph = [[] for _ in range(ALPHABET_SIZE)]
    
    # Build the graph based on the provided sequences
    for i in range(len(firstSeq)):
        u = char_to_index(firstSeq[i])
        v = char_to_index(secSeq[i])
        time = timeSeq[i]
        graph[u].append((v, time))  # u -> v with time
    
    # Use Floyd-Warshall to find the shortest paths between all pairs of characters
    shortest_paths = floyd_warshall(graph)
    
    # Now compute the total time for converting strPwd to strInter
    total_time = 0
    for p_char, i_char in zip(strPwd, strInter):
        p_index = char_to_index(p_char)
        i_index = char_to_index(i_char)
        
        # The minimum time to convert p_char to i_char
        total_time += shortest_paths[p_index][i_index]
    
    return total_time

# Read input
strPwd = input().strip()
strInter = input().strip()
count = int(input().strip())

firstSeq = input().strip().split()
countM = int(input().strip())
secSeq = input().strip().split()
countP = int(input().strip())
timeSeq = list(map(int, input().strip().split()))

# Output the result
print(minimum_conversion_time(strPwd, strInter, firstSeq, secSeq, timeSeq))
subdirs := \
examples/Ch01-Foundamentals/DoublyLinkedList \
examples/Ch01-Foundamentals/ExpressionEvaluation \
examples/Ch01-Foundamentals/IntegerSequence \
examples/Ch01-Foundamentals/IntrusiveLinkedList \
examples/Ch01-Foundamentals/PolynomialAddition \
examples/Ch01-Foundamentals/SinglyLinkedList \
examples/Ch01-Foundamentals/StopWatch \
examples/Ch01-Foundamentals/WeightedQuickUnion \
 \
examples/Ch02-Sorting/HeapSort1-PriorityQueue \
examples/Ch02-Sorting/HeapSort2-IndexPriorityQueue \
examples/Ch02-Sorting/HeapUpdateKey \
examples/Ch02-Sorting/QuickSort1-Sorting \
examples/Ch02-Sorting/QuickSort2-DoublingTime \
examples/Ch02-Sorting/QuickSort3-PartitionSchemes \
 \
examples/Ch03-Searching/BinarySearchTree1-Insertion \
examples/Ch03-Searching/BinarySearchTree2-Deletion \
examples/Ch03-Searching/FrequencyCounter \
examples/Ch03-Searching/SymbolTable \
 \
examples/Ch04-Graphs/BreadthFirstSearch \
examples/Ch04-Graphs/ConnectedComponents \
examples/Ch04-Graphs/DepthFirstSearch1-Recursive \
examples/Ch04-Graphs/DepthFirstSearch2-Nonrecursive \
examples/Ch04-Graphs/DepthFirstSearch3-IncorrectNonrecursive \
examples/Ch04-Graphs/FindingPaths-BFS \
examples/Ch04-Graphs/FindingPaths-DFS \
examples/Ch04-Graphs/MinimumSpanningTree-Kruskal \
examples/Ch04-Graphs/MinimumSpanningTree-PrimUsingLinearSearch \
examples/Ch04-Graphs/MinimumSpanningTree-PrimUsingPriorityQueue \
examples/Ch04-Graphs/ShortestPaths-BellmanFordUsingEdgeList \
examples/Ch04-Graphs/ShortestPaths-BellmanFordUsingQueue \
examples/Ch04-Graphs/ShortestPaths-NegativeCycleDetection \
examples/Ch04-Graphs/ShortestPaths-DijkstraUsingLinearSearch \
examples/Ch04-Graphs/ShortestPaths-DijkstraUsingPriorityQueue \
examples/Ch04-Graphs/StronglyConnectedComponents-Kosaraju \
examples/Ch04-Graphs/StronglyConnectedComponents-Tarjan \
examples/Ch04-Graphs/TopologicalSort \

.PHONY: all $(subdirs)

all: $(subdirs)

$(subdirs):
	$(MAKE) --directory=$@

clean:
	for d in $(subdirs); \
	do \
		$(MAKE) --directory=$$d clean; \
	done

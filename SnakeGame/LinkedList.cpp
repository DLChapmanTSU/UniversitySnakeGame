#include "Snake.h"
#include "LinkedList.h"

//Adds a node to the front of the list containing the given segment
void LinkedList::PushFront(Segment s)
{
	ListNode n;
	n.m_segment = std::make_shared<Segment>(s);
	if (Size() == 0) { //Makes the new node the front and back node if the list is empty
		m_front = std::make_shared<ListNode>(n);
		m_back = m_front;
		return;
	}

	//Makes the next node the current front node
	//Then makes the previous node of the current front node the new node
	//Finally, makes the new node the front node
	n.m_next = m_front;
	m_front->m_previous = std::make_shared<ListNode>(n);
	m_front = m_front->m_previous;
}

//Adds a node to the back of the list containing the given segment
void LinkedList::PushBack(Segment s)
{
	ListNode n;
	n.m_segment = std::make_shared<Segment>(s);
	if (Size() == 0) { //Makes the new node the front and back node if the list is empty
		m_front = std::make_shared<ListNode>(n);
		m_back = m_front;
		return;
	}

	//Makes the previous node the current back node
	//Then, makes the next node of the current back node the new node
	//Finally, makes the new back node the new node
	n.m_previous = m_back;
	m_back->m_next = std::make_shared<ListNode>(n);
	m_back = m_back->m_next;
}

//Iterates through the list to count each node and return the result
int LinkedList::Size() const
{
	//Returns 0 if the front node doesn't exist
	if (m_front == nullptr) {
		return 0;
	}

	//Creates a temporary node to traverse the list
	std::shared_ptr<ListNode> current = m_front;
	bool endFound{ false };
	int nodeCount{ 0 };

	while (endFound == false)
	{
		//Increments the node count
		nodeCount++;
		
		//Ends the loop if the current node is the last node
		if (current == m_back) {
			endFound = true;
		}
		else { //Otherwise, current node hops to the next node
			current = current->m_next;
		}
	}

	return nodeCount;
}

//Removes the last node from the list
void LinkedList::PopBack()
{
	//Resets the back and front pointer if only one node is in the list
	if (m_back == m_front) {
		m_back.reset();
		m_front.reset();
		return;
	}

	//Creates a temporary pointer at the back node
	//Moves the back pointer to the previous node
	//Resets the next pointer on the new back node
	//Resets the previous pointer of the old back node
	//Finally, the temporary node is reset
	std::shared_ptr<ListNode> temp = m_back;
	m_back = m_back->m_previous;
	m_back->m_next.reset();
	temp->m_previous.reset();
	temp.reset();
}

//Gets a reference to a given node
//Should not be called if the list is empty
std::shared_ptr<Segment>& LinkedList::GetNode(int i)
{
	if (i >= Size()) {
		i = Size() - 1;
	}
	else if (i < 0) {
		i = 0;
	}

	if (i == 0) {
		return m_front->m_segment;
	}

	if (i + 1 == Size()) {
		return m_back->m_segment;
	}

	std::shared_ptr<ListNode> current;

	//Iterates through the list until the given node (i) is reached
	if (i <= (Size() / 2)) {
		//Creates a temporary pointer to traverse the list
		current = m_front;
		for (int j = 0; j < i; j++)
		{
			current = current->m_next;
		}
	}
	else {
		//Creates a temporary pointer to traverse the list
		current = m_back;
		for (int j = Size() - 1; j > i; j--)
		{
			current = current->m_previous;
		}
	}

	_ASSERT(current != nullptr);
	_ASSERT(current->m_segment != nullptr);
	return current->m_segment;
}

//Returns the index of the last segment in the list
int LinkedList::GetBackIndex()
{
	return m_back->m_segment->GetIndex();
}

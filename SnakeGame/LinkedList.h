#pragma once

#include <iostream>
#include <vector>

class Segment;

//Class for my custom linked list
//Used by the snakes for handling their individual segments
class LinkedList {
protected:
	struct ListNode {
		std::shared_ptr<ListNode> m_next{ nullptr };
		std::shared_ptr<ListNode> m_previous{ nullptr };
		std::shared_ptr<Segment> m_segment;
	};
private:
	std::shared_ptr<ListNode> m_front{ nullptr };
	std::shared_ptr<ListNode> m_back{ nullptr };
public:
	void PushFront(Segment s);
	void PushBack(Segment s);
	int Size() const;
	void PopBack();
	std::shared_ptr<Segment>& GetNode(int i);
	int GetBackIndex();
};
#include "Sequence.h"

Sequence::Sequence(): m_size(0) // the linked list has zero items at the time of creation
{
    // the following implementation creates a circular doubly linked list which is empty (i.e. has only the dummy node). A pictorial representation can be found in the report.
    
    head = new Node;
    head->m_next = head;
    head->m_prev = head;
}

Sequence::Sequence(const Sequence& other)
{
    // as a first step to copying one Sequence object into another, we copy other other.m_size to *this's m_size.
    m_size = other.m_size;
    
    // next, we initialize head to be a new dynamic Node object, and adjust its pointers so that the setup now represents an empty linked list.
    head = new Node;
    head->m_next = head;
    head->m_prev = head;
    
    // this time, we'll need two pointers of type Node. One of them, p, will be needed to traverse through *this's linked list, and another, p_other, to traverse through other's linked list. We initialize p to be head because we'll need to perform insertion operations over *this's linked list, i.e. add new items, and p_other to be other.head->m_next, because we need to read in values from each Node object of other's linked list.
    
    Node* p;
    Node* p_other = other.head->m_next;
    int i=0; // the integer i is used to keep track of where we are in the linked list (both *this's and other's).
    
    for(p=head; i<m_size; p = p->m_next)
    {
        // the following algorithm inserts a new Node object into *this's linked list and copies over the m_data attribute of the corresponding Node object in other's linked list. The pointers of p and head are appropriately adjusted to ensure that there are no dangling pointers. Also, essentially, every time the loop runs, three things are being updated - i, p, and p_other.
        Node* newItem = new Node;
        newItem->m_data = p_other->m_data;
        p->m_next = newItem;
        newItem->m_prev = p;
        newItem->m_next = head;
        head->m_prev = newItem;
        p_other = p_other->m_next;
        i++;
    }
}

Sequence::~Sequence()
{
    // once again, we need a pointer p of type Node and an integer i to respectively traverse through the linked list and keep track of where we are in the linked list. For efficiency, we begin traversing from the rear of the linked list (i.e. the last item). Thus, we initialize p to be head->m_prev.
    Node* p;
    int i=0;
    
    for(p=head->m_prev; i<m_size; i++)
    {
        // the following algorithm appropriately creates another pointer of type Node temp which is set equal to p and perform deletion using temp (and set p to p->m_prev because if we don't it will be left dangling, and we don't want that because we are using p to traverse through the linked list). We appropriately adjust pointers of head and temp->m_prev so that their corresponding Node objects are linked with each other. After having done so, we can delete the Node object pointed to at by temp.
        Node* temp = p;
        p=p->m_prev;
        temp -> m_prev -> m_next = head;
        head -> m_prev = temp -> m_prev;
        delete(temp);
    }
    
    // after the end of this loop, we'll be left with an empty linked list, which means that it is just the head pointer pointing to the dummy node. Thus, we delete the dummy node using delete operation on head.
    
    delete(head);
    
}

Sequence& Sequence::operator=(const Sequence&other)
{
    // if *this and other are the same objects, we don't need to do anything, and we can plainly return *this, otherwise we perform the following algorithm.
    if(this!=&other)
    {
        // we create a temporary Sequence object and assign it all characteristics of other using the copy constructor.
        Sequence temp = other;
        // next, we swap *this and temp using the already created swap function. This results in *this acquiring all features of other and temp acquiring all features of *this.
        swap(temp);
    }
    // since temp is local to this function, when we exit from this function, temp's destructor is called, which deletes any dynamically allocated memory which previously belongs to temp (and previously belonged to *this). This eliminates any potential chances for memory leaks.
    return(*this);
}

bool Sequence::empty() const
{
    // to check if a Sequence object is empty, we simply assess the value of m_size, which saves us execution time by not having to loop through the linked list to figure out how many items there are in the list.
    if(m_size==0)
    {
        return(true);
    }
    return(false);
}

int Sequence::size() const
{
    // returns the number of items currently held in the linked list.
    return(m_size);
}

int Sequence::insert(int pos, const ItemType& value)
{
    if(pos>=0 && pos<=m_size) // legal values of pos could be 0 to m_size (the latter inclusive because it means insertion at the end of the linked list.
    {
        Node* p; // create a pointer to walk through the linked list until the desired position is reached.
        int i=0; // to keep track of which position the m_next pointer of p is pointing to.
   
        // we initialize p to be head and not head->m_next because in order to insert a new Node at a particular position, one needs to have p pointing at a position one before that.
        
        for(p = head; i<pos; p=p->m_next)
        {
            i++; // every time the loop runs successfully, i is incremented by one and p is made to point to the next node object.
        }
        
        // when the loop breaks, p will be pointing at a Node object which is at position (pos-1) (assuming the first node address, not the dummy one, is 0). The following algorithm will then insert a new node at pos.
        
        // under the following algorithm, a new Node object is dynamically created and the pointers of the Node object pointed to by p, p->m_next, as well as newItem are appropriately adjusted so that there are no dangling pointers left and the newly created Node object is inserted at the position pos. Note that there is no need to consider potential problems with null pointers because the circular doubly linked list doesn't use them in any way.
        
        Node* newItem = new Node;
        newItem->m_data = value;
        newItem->m_next = p->m_next;
        newItem->m_prev = p;
        p->m_next = newItem;
        newItem->m_next->m_prev = newItem;
        m_size++; // m_size must be incremented every time a new item is successfuly inserted.
        return(pos); // returns the position where object was inserted after successful insertion.
    }
    return(-1);
}

int Sequence::insert(const ItemType& value)
{
    Node* p; // similar to the first insert function, we create a pointer of type Node to traverse through the linked list, initializing it below at head for the same reasons as in the first insert function.
    
    int i = 0; // we create a counter variable i to keep track of the current position of the Node object pointed at by p->m_next within the linked list.
    
    for(p = head; i<m_size; p=p->m_next)
    {
        // if m_data of p->m_next satisfies the desired conditions, then that is the position where the new Node object needs to be inserted. As is ideal, p is pointing to a Node object just before that, so we can safely exit our loop to insert a new Node object in the linked list as done before in the first insert function.
        
        if(value<=p->m_next->m_data)
        {
            break;
        }
        i++;
    }
    
    // the algorithm for inserting a new object is the same as the first insert function, and works appropriately because p is pointing to a Node object that is just one before where we need to insert our new Node object.
    
    Node* newItem = new Node;
    newItem->m_data = value;
    newItem->m_next = p->m_next;
    newItem->m_prev = p;
    p->m_next = newItem;
    newItem->m_next->m_prev = newItem;
    m_size++;
    
    return(i); // i was the location where the new Node object was inserted, it is returned. Note that due to the test conditions of the loop, i has to be within legal bounds.
}

bool Sequence::erase(int pos)
{
    if(pos>=0 && pos<m_size) // legal values for pos are from 0 to m_size-1.
    {
        
        // we create a walking pointer of type Node and a counter integer i to keep track of the position which Node object p is pointing to. Note that in this case, we will initialize p to be head->m_next (and not head, unlike insert functions), since the deletion algorithm needs p to point at the Node object which needs to be deleted.
        Node* p;
        int i=0;
        
        // we traverse through the linked list until p points at the desired Node object which has to be deleted.
        
        for(p = head->m_next; i<pos; p = p->m_next)
        {
            i++;
        }
        
        // the following algorithm appropriately adjusts the pointers of the Node objects pointed at by p->m_prev and p->m_next so that these two Nodes are connected with each other, and after doing so, the Node object pointed to at by p can safely be deleted (without running any risk of a dangling pointer, because we appropriately linked the successor and predecessor nodes already).
        
        p->m_prev->m_next = p->m_next;
        p->m_next->m_prev = p->m_prev;
        delete(p);
        
        m_size--; // m_size must be decreased every time an object is deleted.
        
        return(true);
    }
    return(false);
}

int Sequence::remove(const ItemType& value)
{
    
    // again, we create a pointer of type Node to traverse through the list and an integer i to keep track of where p is pointing to. Herein, we additionally declare a counter variable to keep track of how many instances of the function's parameter 'value' have been found. Obviously, counter must be intialized to 0.
    
    Node* p;
    int i = 0;
    int counter = 0;
    
    // we keep traversing through the linked list as long as it has items. Note that m_size may be reduced even while traversing through the linked list, as every time a Node object is deleted, m_size is reduced by one.
    
    for(p = head->m_next; i<m_size; p = p->m_next)
    {
        // if m_data of the node pointed to at by p is indeed equal to value, then that Node object must be deleted.
        if(p->m_data == value)
        {
            // since we may have to keep traversing through the linked list even after this iteration of the loop, we cannot use delete(p), as that is the pointer we are using to traverse through the linked list. Next, we update p to point to the Node object before it Therefore, we declare another pointer temp of type Node, and fix the m_next and m_prev pointers of the Node objects before and after the present Node object so that these two are properly connected. Note that when p is updated to p->m_next in the next iteration, it will point to the Node object just after the recently deleted one, which is ideal behavior.
            Node* temp = p;
            p = p->m_prev;
            temp->m_prev->m_next = temp->m_next;
            temp->m_next->m_prev = temp->m_prev;
            delete(temp);
            m_size--; // m_size is reduced by 1 after successful deletion of the present Node.
            i--;
            counter++; // counter must be incremented as one instance of 'value' was deleted.
        }
        i++;
    }
    return(counter);
}

bool Sequence::get(int pos, ItemType& value) const
{
    if(pos>=0 && pos<m_size) // legal values of pos are from 0 to m_size-1.
    {
        // we create a pointer of type Node and an integer to traverse through the linked list until we reach the desired position. Note that we initialize p to be head->m_next, as we actually need to dereference m_value from the Node object pointed to at by p.
        int i=0;
        Node* p;
        for(p = head->m_next; i<pos; p = p->m_next)
        {
            i++;
        }
        // now that p points to a Node object whose position is pos, we can copy over its m_data member variable to the parameter value, which was passed by reference, and is hence mutable.
        value = p->m_data;
        return(true);
    }
    return(false);
}

bool Sequence::set(int pos, const ItemType& value)
{
    // the algorithm of set() is almost identical to that of get(),the only difference being that instead of copying over p->m_data to value when appropriate (as done in get()), we now instead copy over value to p->m_data when appropriate (i.e. when p is pointing to a Node object at position pos).
    
    if(pos>=0 && pos<m_size)
    {
        int i=0;
        Node* p;
        for(p = head->m_next; i<pos; p = p->m_next)
        {
            i++;
        }
        p->m_data = value;
        return(true);
    }
    return(false);
}

int Sequence::find(const ItemType& value) const
{
    // once again, we create an integer i to keep track of our position within the linked list and a pointer p of type Node to traverse through the linked list. Notice that we need to initialize p at head->m_next because we need to asses m_data of each Node object.
    
    int i=0;
    
    for(Node* p = head->m_next; i<m_size; p=p->m_next)
    {
        // as soon as the m_data member variable of the Node object pointed to at by p is found to be the same as value, we simply return i, which is the position of that Node, and break out of the function.
        if(p->m_data==value)
        {
            return(i);
        }
        i++;
    }
    // if the loop ran successfuly, it just means that 'value' wasn't found in the linked list, so we return -1.
    return(-1);
}

void Sequence::swap(Sequence& other)
{
    // we swap the m_size member variables of the the Sequences *this and other by means of another integer called temp_m_size as follows.
    int temp_m_size = other.m_size;
    other.m_size = m_size;
    m_size = temp_m_size;
    
    // next, in order to swap the linked lists, we simply swap the head pointers by means of another temporary pointer, temp_head. The consequence of this will be that accessing either linked list (which can only be done through head), will now lead to accessing the other linked list.
    
    Node* temp_head = other.head;
    other.head = head;
    head = temp_head;
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
    // if either of seq1 or seq2 are empty, it is obvious that a seq2 cannot be found as subsequence in seq1. Besides, seq1 needs to be the same in size or larger than seq2 for seq2 to be found in it. So if either of these conditions is false, we skip the if condition and directly return -1.
    if(seq1.size()>0 && seq2.size()>0 && seq1.size()>=seq2.size())
    {
        ItemType seq1present; // for keeping track of what the value of the present seq1 item is (in the outer loop).
        
        ItemType seq2first; // the very first item of seq2.
        seq2.get(0, seq2first);
        
        ItemType seq1test; // for keeping track of what the value of the present seq1 item is for the inner loop.
        ItemType seq2test; // for keeping track of what the value of the present seq2 item is for the inner loop.
        
        
        
        for(int i=0; i<seq1.size(); i++)
        {
            seq1.get(i, seq1present);
            if(seq1present == seq2first) // as soon as the first element of seq2 is matched with some element of seq1.
            {
                
                bool b = true; // we assume initially that seq2 will be found as a subsequence in seq1.
                
                int j;
                
                for(j = 1; j<seq2.size() && i+j<seq1.size(); j++) // conditions check that j shouldn't run out of seq2's size and the corresponding indeces used for seq1 shouldn't run over seq1's size either.
                {
                    seq1.get(i+j, seq1test);
                    seq2.get(j, seq2test);
                    
                    if(seq1test != seq2test) // as soon as the matching proves false, we set b to false and break out of the loop.
                    {
                        b = false;
                        break;
                    }
                }
                
                if(i+j==seq1.size() && j!=seq2.size()) // this would mean that although we had managed to match a specific number of elements between seq1 and seq2, we ran out of seq1 but not of seq2, seq2 cannot be concluded to be a subsequence of seq1.
                {
                    b = false;
                }
                
                if(b) // b would be true if and only if the matching above succeeded, and if it did, i, where the first elements of seq1 and seq2 matched, should be returned. Note that i would correspond to the position where seq2 was first started as a subsequence in seq1.
                {
                    return(i);
                }
            }
        }
    }
    return(-1); // if one hasn't returned anything from within the for loop inside the if statement above, it simply means that either the size of one of seq1, seq2, or both was/ were 0, or seq2 wasn't found as a subsequence in seq1. In all these cases, we simply return -1.
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
    // to avoid aliasing issues, we create a new Sequence object called temp and formulate the resulting sequence due to interleaving within it. This avoids making any changes to seq1 and seq2 while doing so. Once done formulating temp according to given specs, we simply overwrite result using the assignment operator.
    Sequence temp;
    
    // we subdivide are dealing with seq1 and seq2 into the following 5 sub-cases.
    
    // if both are of the same size.
    if(seq1.size() == seq2.size())
    {
        // the following two are for copying over each successive item in seq1 and seq2.
        ItemType seq1present;
        ItemType seq2present;
        
        for(int i=0; i<seq1.size(); i++)
        {
            seq1.get(i, seq1present);
            seq2.get(i, seq2present);
            // we insert two items at a time into temp, the first being from seq1 and the next being from seq2. Inserting at 2*i and 2*i + 1 ensures that two items are inserted into temp each time the loop runs and temp gets completed appropriately.
            temp.insert(2*i, seq1present);
            temp.insert((2*i)+1, seq2present);
        }
    }
    
    else if(seq1.size() < seq2.size())
    {
        ItemType seq1present;
        ItemType seq2present;
        
        // we first fill up temp till 2*seq1.size(), so that it has all elements from seq1 and seq1.size() elements from seq2 in alternation. This code is quite similar to the first case.
        
        for(int i=0; i<seq1.size(); i++)
        {
            seq1.get(i, seq1present);
            seq2.get(i, seq2present);
            temp.insert(2*i, seq1present);
            temp.insert((2*i)+1, seq2present);
        }
        
        // next, we simply fill temp with the remaining elements of seq2.
        
        for(int j=0; j<(seq2.size()-seq1.size()); j++)
        {
            seq2.get(seq1.size()+j, seq2present);
            temp.insert(2*seq1.size()+j, seq2present);
        }
    }
    
    else if(seq2.size() < seq1.size())
    {
        // this code is just an inversion of the previous case; it is identical in logic.
        
        ItemType seq1present;
        ItemType seq2present;
        
        for(int i=0; i<seq2.size(); i++)
        {
            seq1.get(i, seq1present);
            seq2.get(i, seq2present);
            temp.insert(2*i, seq1present);
            temp.insert((2*i)+1, seq2present);
        }
        
        for(int j=0; j<(seq1.size()-seq2.size()); j++)
        {
            seq1.get(seq2.size()+j, seq1present);
            temp.insert(2*seq2.size()+j, seq1present);
        }
    
    }
    
    else if(seq1.size() == 0)
    {
        temp = seq2; // if seq1 is empty, temp simply becomes seq2.
    }
        
    else if(seq2.size() == 0)
    {
        temp = seq1; // if seq2 is empty, temp simply becomes seq1.
    }
    
    // if the sizes of both seq1 and seq2 are empty, none of the if/ else if blocks above will run, so temp will remain a newly created, unaltered Sequence object (which means that it will be empty). Thus, if both seq1 and seq2 are empty, result will be set to temp which is also an empty Sequence.
    
    result = temp; // we write over result, irrespective of its previous value, with temp, using the assignment operator. 
}


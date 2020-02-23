// $Id: listmap.tcc,v 1.15 2019-10-30 12:44:53-07 - - $
//Kevin Wu kwu30@ucsc.edu, Daniel Nguyen dnguy182@ucsc.edu

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));
   while(!empty()){ //keeps rasing begin until all empty
   		erase(begin());
   }
}

//
// iterator listmap::insert (const value_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);
   //pair is curr (key,val), what was just inputed rn
   node* n = new node(nullptr, nullptr, pair);
   iterator itor = begin(); //sets itor to start of list

   while(itor!=end() && less(itor->first,pair.first)){ 
   		//itor!=end keeps iterating until anchor reached again(looped)
   		// les(...) returns true if right is bigger
   		++itor; //goes to next node until right pos found
   }
   //if itor and pair are equal, resets itor
   if(itor!=end() && !less(pair.first,itor->first)){
      	itor = erase(itor);
   }
   //Pointer reassignments
   n->next = itor.where; //new node n->next points to itor
   n->prev = itor.where->prev; // new node n->prev points to itor->prev
   itor.where->prev->next = n; //itor->prev->next points back to n
   itor.where->prev = n; //itor->prev points to newnode n
   //cout << "itor->second: " << itor->first<<endl;
   return iterator(n);
}

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   DEBUGF ('l', that);
   iterator itor = begin();
   for (itor = begin(); itor != end(); ++itor) {
  	//if (itor->first == that) { //?<edit>
      if(!less(itor->first,that) && !less(that,itor->first)){
      		break;
    	}
   }
   return itor;
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   DEBUGF ('l', &*position);
   if(position.where==nullptr){
   		//cout << "error in erase"<<endl;
   		exit(1);
   }
   //points temp to position we want to remove
   iterator temp = position;
   position.where->next->prev=position.where->prev;
   position.where->prev->next=position.where->next;
   ++ position; //position changes to next
   delete temp.where; //deletes node we want to remove
   return position; //returns new position
}



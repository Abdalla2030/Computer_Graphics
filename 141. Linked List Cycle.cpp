// 16/03/2022
// linked list cycle
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    bool hasCycle(ListNode *head) {
        int i = 0;
        ListNode* it = head;
        // if it arrives to 10^4 + 1 without break from loop 
        // there is a cycle
        while(it!=NULL && i<10001)
        {
            it = it->next;
            i++;
        }
        // true if i == 10001 that means there is a cycle
        return (i==10001);
    }
};
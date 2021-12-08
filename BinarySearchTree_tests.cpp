// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"


TEST(test_stub) {
    BinarySearchTree<int> bruh;
    bruh.insert(5);
    ASSERT_TRUE(bruh.size()==1);
    bruh.insert(3);
    bruh.insert(7);
    bruh.insert(2);
    bruh.insert(4);
    bruh.insert(6);
    bruh.insert(8);
    bruh.insert(9);
    ASSERT_TRUE(bruh.height() == 4);
    ASSERT_TRUE(bruh.find(9) != bruh.end());
    ASSERT_TRUE(bruh.find(2) != bruh.end());
    ASSERT_TRUE(bruh.find(1) == bruh.end());
    ASSERT_TRUE(*bruh.min_element() == 2);
    ASSERT_TRUE(*bruh.begin() == 2);
    ASSERT_TRUE(*bruh.min_greater_than(2)==3);
    ASSERT_TRUE(bruh.size() == 8);
    ASSERT_TRUE(bruh.check_sorting_invariant());

    BinarySearchTree<int> aye(bruh);
    ASSERT_FALSE(aye.empty());
    aye.insert(1);
    std::ostringstream ayeYO;
    aye.traverse_inorder(ayeYO);
    ASSERT_EQUAL(ayeYO.str(),"1 2 3 4 5 6 7 8 9 ");
    std::ostringstream ayeWHAT;
    *aye.find(9) = 10;
    aye.traverse_preorder(ayeWHAT);
    ASSERT_EQUAL(ayeWHAT.str(),"5 3 2 1 4 7 6 8 10 ");
    auto it = aye.begin();
    ASSERT_TRUE(*++++it == 3);
    it = aye.find(10);
    ASSERT_TRUE(++it == aye.end());
    it = aye.begin();
    *it = 3;
    ASSERT_FALSE(aye.check_sorting_invariant());
    BinarySearchTree<int> gabe = aye;
    

}

TEST_MAIN()

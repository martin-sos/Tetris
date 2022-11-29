#pragma once
template <class T>
class Bag_of_Seven
{
public:
    Bag_of_Seven(T one, T two, T three, T four, T five, T six, T seven)
        :list(std::vector<T>{one, two, three, four, five, six, seven}),
        bag(list)
    {
    }

    // simulates grabbing uniformly at random one item from a bag of (initially) seven elements
    // once grabbed, the item is not available anymore, and the bag contains one element less
    // if the bag is empty, it will be refilled with the initial 7 elements
    T grab_from_bag() 
    {
        if (bag.empty())
        {
            bag = list;
        }
        
        int random_index = (std::rand() % bag.size());
        T grab = bag[random_index];
        bag.erase(bag.begin() + random_index);
        return grab;
    }

    void clear()
    {
        bag.clear();
    }


private:
    const std::vector<T> list;
    std::vector<T> bag;
};


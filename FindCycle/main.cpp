#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using node_t = std::string;
using node_list_t = std::vector<node_t>;
using node_map_t = std::map<node_t, node_list_t>;
using node_set_t = std::set<node_t>;
using cycle_list_t = std::vector<node_list_t>;

node_map_t load_vector(const std::string& text)
{
    node_map_t node_map;
    std::string_view src(text);
    for (auto pos = src.find(','); ; pos = src.find(',')) {
        auto index = src.find("->");
        node_t key(src, 0, index);
        if (pos == std::string::npos) {
            node_t value(src.data() + index + 2);
            node_map[key].push_back(value);
            break;
        } else {
            node_t value(src, index + 2, pos - index - 2);
            node_map[key].push_back(value);
            src.remove_prefix(pos + 1);
        }
    }

    return node_map;
}

int find_node(const node_list_t& nodes, const node_t& key)
{
    for (int i = 0; i < nodes.size(); ++i)
        if (nodes[i] == key)
            return i;

    return -1;
}

void extract_cycle(cycle_list_t& result, node_set_t& keys, node_list_t path, const node_map_t& node_map, const node_list_t& nodes)
{
    for (const auto& node : nodes) {
        int index = find_node(path, node);
        if (index < 0) {
            auto iter = node_map.find(node);
            if (iter != node_map.end() && keys.find(node) == keys.end()) {
                node_list_t p(path);
                p.push_back(iter->first);
                extract_cycle(result, keys, p, node_map, iter->second);
            }
        } else {
            // if (index == 0)
            {
                result.emplace_back(path.begin(), path.end());
                keys.insert(path.begin(), path.end());
            }
        }
    }
}

cycle_list_t extract_cycle(const node_map_t& node_map)
{
    cycle_list_t result;
    node_set_t keys;
    for (const auto& pair : node_map)
    {
        if (keys.find(pair.first) == keys.end()) {
            extract_cycle(result, keys, node_list_t{ pair.first }, node_map, pair.second);
            keys.insert(pair.first);
        }
    }

    return result;
}

void show_cycle(const cycle_list_t& cycles) {
    for (const auto& cycle : cycles) {
        for (const auto& node : cycle) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    /**
     *        T<--P<----Q<-J
     *            ¡ý        ¡ü
     *   K->G->C->A->E->F->B->H->I
     *      ¡ü  ¡ý  ¡ü  ¡ý     ¡ý 
     *      D<-L<-|--M<-N<-O
     *      ¡ý     |     ¡ü
     *      X->Y->Z->R->S
     * 
     */
    std::string text =
        "K->G,G->C,C->A,A->E,E->F,F->B,B->H,"
        "H->I,B->J,J->Q,Q->P,P->T,Z->R,R->S,"
        "P->A,B->O,O->N,N->M,M->L,L->D,D->G,"
        "C->L,E->M,D->X,X->Y,Y->Z,Z->A,S->N";

    auto node_map = load_vector(text);
    auto result = extract_cycle(node_map);

    show_cycle(result);

    return 0;
}








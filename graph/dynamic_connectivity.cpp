#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;

uint64_t random_address() { char *p = new char; delete p; return uint64_t(p); }

struct safe_hash {
    static unsigned hash32(unsigned x) {
        x = (x ^ (x >> 16)) * 0x45d9f3b;
        x = (x ^ (x >> 16)) * 0x45d9f3b;
        return x ^ (x >> 16);
    }

    static uint64_t splitmix64(uint64_t x) {
        // http://xorshift.di.unimi.it/splitmix64.c
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    template<typename T>
    uint64_t operator()(T x) const {
        static const uint64_t FIXED_RANDOM = splitmix64(chrono::steady_clock::now().time_since_epoch().count() * (random_address() | 1));
        return sizeof(x) <= 4 ? hash32(unsigned(x ^ FIXED_RANDOM)) : splitmix64(x ^ FIXED_RANDOM);
    }
};

struct safe_pair_hash {
    template<typename T_pair>
    uint64_t operator()(const T_pair &x) const {
        static const uint64_t RANDOM_MULT = safe_hash::splitmix64(chrono::steady_clock::now().time_since_epoch().count() * (random_address() | 1)) | 1;
        static safe_hash hash_obj;
        return hash_obj(RANDOM_MULT * x.first + x.second);
    }
};

const int INF = int(1e9) + 5;

struct dynamic_connectivity {
    struct undo_op {
        int x, y, y_data;

        undo_op() {}

        undo_op(int _x, int _y, int _y_data) : x(_x), y(_y), y_data(_y_data) {}
    };

    struct dc_union_find {
        // When data[x] < 0, x is a root and -data[x] is its tree size. When data[x] >= 0, data[x] is x's parent.
        vector<int> data;
        int components = 0;

        dc_union_find(int n = -1) {
            if (n >= 0)
                init(n);
        }

        void init(int n) {
            data.assign(n + 1, -1);
            components = n;
        }

        int find(int x) const {
            // Note that we don't use path compression since it doesn't play well with undo operations.
            while (data[x] >= 0)
                x = data[x];

            return x;
        }

        int get_size(int x) {
            return -data[find(x)];
        }

        bool unite(int x, int y, vector<undo_op> &_undo_ops) {
            x = find(x);
            y = find(y);

            if (x == y)
                return false;

            if (-data[x] < -data[y])
                swap(x, y);

            _undo_ops.emplace_back(x, y, data[y]);
            data[x] += data[y];
            data[y] = x;
            components--;
            return true;
        }

        void undo(const undo_op &op) {
            components++;
            data[op.y] = op.y_data;
            data[op.x] -= op.y_data;
        }
    };

    struct query {
        // type: +1 = add, -1 = erase, 0 = query
        // when type = 0, we can add any custom query that union-find supports:
        // x < 0: count components
        // x >= 0, y < 0: give component size at x
        // x, y >= 0: are x, y in the same component
        int type, x, y, matching_index;

        query(int _type = -INF, int _x = -INF, int _y = -INF, int _matching_index = INF)
            : type(_type), x(_x), y(_y), matching_index(_matching_index) {}
    };

    vector<query> queries;
    gp_hash_table<pair<int, int>, pair<int, int>, safe_pair_hash> seen_table;
    dc_union_find UF;
    vector<undo_op> undo_ops;
    vector<int> answers;

    // When calling recurse(start, end), all of the edges that should exist for the entire range [start, end) have
    // already been united and are in undo_ops.
    void recurse(int start, int end) {
        if (end - start <= 1) {
            if (end - start == 1 && queries[start].type == 0) {
                if (queries[start].x < 0)
                    answers.push_back(UF.components);
                else if (queries[start].y < 0)
                    answers.push_back(UF.get_size(queries[start].x));
                else
                    answers.push_back(UF.find(queries[start].x) == UF.find(queries[start].y));
            }

            return;
        }

        int mid = (start + end) / 2;
        size_t initial_size = int(undo_ops.size());

        // To recurse on [start, mid), unite all edges that occur before [start, mid) and end within [mid, end).
        for (int i = mid; i < end; i++)
            if (queries[i].type == -1 && queries[i].matching_index < start)
                UF.unite(queries[i].x, queries[i].y, undo_ops);

        recurse(start, mid);

        while (undo_ops.size() > initial_size) {
            UF.undo(undo_ops.back());
            undo_ops.pop_back();
        }

        // To recurse on [mid, end), unite all edges that occur in [start, mid) and end after [mid, end).
        for (int i = start; i < mid; i++)
            if (queries[i].type == +1 && queries[i].matching_index >= end)
                UF.unite(queries[i].x, queries[i].y, undo_ops);

        recurse(mid, end);

        while (undo_ops.size() > initial_size) {
            UF.undo(undo_ops.back());
            undo_ops.pop_back();
        }
    }

    void add_components_query() {
        queries.emplace_back(0);
    }

    void add_component_size_query(int x) {
        queries.emplace_back(0, x);
    }

    void add_same_query(int x, int y) {
        queries.emplace_back(0, x, y);
    }

    void add_edge_op(int type, int x, int y) {
        assert(type == +1 || type == -1);

        if (x > y)
            swap(x, y);

        if (type == +1) {
            pair<int, int> &seen = seen_table[{x, y}];

            // seen.second is the current frequency of this edge.
            if (seen.second++ == 0) {
                seen.first = int(queries.size());
                queries.emplace_back(type, x, y, INF);
            }
        } else {
            pair<int, int> &seen = seen_table[{x, y}];
            assert(seen.second > 0);

            if (--seen.second == 0) {
                int index = seen.first;
                seen.first = -1;
                queries[index].matching_index = int(queries.size());
                queries.emplace_back(type, x, y, index);
            }
        }
    }

    vector<int> solve(int n) {
        UF.init(n);
        recurse(0, int(queries.size()));
        return answers;
    }
};
/*
add_edge_op(+1/-1 , u , v)  -> Add/Remove u-v edge
add_component_size_query(x) -> Size of the component of x
add_components_query()      -> Number of connected components
add_same_query(u , v)       -> Are u and v in the same component
*/

#ifndef LDPC_H
#define LDPC_H
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class LDPC
{
    private:
        int row_num,column_num;
        int row_ones_num,column_ones_num;
        int index_temp;
        vector<vector<int>> parity_check_matrix;
        vector<vector<int>> bit_node_index,check_node_index;//Index to bit_node and check_node
        int not_used;



    public:
        LDPC();
        void set_parity_check_matrix();
        void set_parity_check_matrix_from_file();
        void string_tidy_up(string&);
        void print_parity_check_matrix();
        vector<vector<int>> get_parity_check_matrix(){return parity_check_matrix;}
        int get_row_num(){return row_num;}
        int get_column_num(){return column_num;}
        int get_row_ones_num(){return row_ones_num;}
        int get_column_ones_num(){return column_ones_num;}
        vector<vector<int>> get_bit_node_index(){return bit_node_index;}
        vector<vector<int>> get_check_node_index() {return check_node_index;}
};

#endif // LDPC_H



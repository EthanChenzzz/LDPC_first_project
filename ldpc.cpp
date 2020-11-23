#include "ldpc.h"

LDPC::LDPC()
{

}
void LDPC::set_parity_check_matrix()
{
    //Initialize parity_check_matrix container
    cout << "Input PCM row number and column number" << endl;
    cin >> column_num >> row_num;

    //Get ones index
    cout << "Input number of ones in each row and each column" <<endl;
    cin >> column_ones_num >> row_ones_num ;

    //input row_ones_num and column_ones_num repeatedly to match the input format of Mackay's. However these input info. are not used in my code.
    cout << "Input row_ones_num and column_one_num repeatedly" <<endl;
    for(int i=0;i<column_num;i++)
        cin >> not_used;
    for(int i=0;i<row_num;i++)
        cin >> not_used;

    //Input index of all "1" in each column
    bit_node_index.resize(column_num,vector<int>(column_ones_num,0));
    cout << "Input index of all '1' in each column" <<endl;
    for (int i=0;i<column_num;i++){
        for (int j=0;j<column_ones_num;j++){
            cin >> index_temp;
            bit_node_index[i][j] = index_temp;
        }
    }

    //Input index of all "1" in each row
    check_node_index.resize(row_num,vector<int>(row_ones_num,0));
    cout << "Input index of all '1' in each row" <<endl;
    for (int i=0;i<row_num;i++){
        for (int j=0;j<row_ones_num;j++){
            cin >> index_temp;
            vector<int> temp_vec_1 = {i+1,index_temp};
            parity_check_matrix.push_back(temp_vec_1);
            check_node_index[i][j] = index_temp;
        }
    }
}
void LDPC::set_parity_check_matrix_from_file()
{
    vector<string> input_all_data;
    string tempa;
    ifstream infile;
    ofstream outfile;
    //infile.open("C:\\Users\\spurp\\qtcrea\\Mid_TEstbech.txt");
    //infile.open("C:\\Users\\spurp\\qtcrea\\mid_1000bench.txt");
    //infile.open("C:\\Users\\spurp\\qtcrea\\TestBench.txt");
    infile.open("C:\\Users\\spurp\\qtcrea\\Big Testbench.txt");
    //infile.open("C:\\Users\\spurp\\qtcrea\\quant14bench.txt");
    //infile.open("C:\\Users\\spurp\\qtcrea\\84bench.txt");
    if(!infile)
        cout << "fail" << endl;
    while (infile >> tempa)
    {
        input_all_data.push_back(tempa);
    }
    column_num = stoi(input_all_data[0]);
    row_num = stoi(input_all_data[1]);
    column_ones_num = stoi(input_all_data[2]);
    bit_node_index.resize(column_num,vector<int>(column_ones_num,0));
    row_ones_num = stoi(input_all_data[3]);
    check_node_index.resize(row_num,vector<int>(row_ones_num,0));
    int index_to_get = column_num + row_num + 4;
    for(int i=0;i<column_num;i++){
        for (int j=0;j<column_ones_num;j++){
            string_tidy_up(input_all_data[index_to_get]);
            bit_node_index[i][j] = stoi(input_all_data[index_to_get]);
            index_to_get = index_to_get + 1;
        }
    }
    for(int i=0;i<row_num;i++){
        for (int j=0;j<row_ones_num;j++){
            string_tidy_up(input_all_data[index_to_get]);
            check_node_index[i][j] = stoi(input_all_data[index_to_get]);
            vector<int> temp_vec_1 = {i+1,stoi(input_all_data[index_to_get])};
            parity_check_matrix.push_back(temp_vec_1);
            check_node_index[i][j] = stoi(input_all_data[index_to_get]);
            index_to_get = index_to_get + 1;
        }
    }

    infile.close();
}
void LDPC::string_tidy_up(string &s)
{
    string::size_type pos = 0;
    while(pos != string::npos)
    {
        pos = s.find_first_of("\r\n", pos);
        if(pos != string::npos)
        s.erase(pos, 1);
    }
    while (s.at(s.length()-1) == ' ')
    {
        s.erase(s.length()-1, 1);
    }
}
void LDPC::print_parity_check_matrix()
{
    cout << "Parity Check Matrix: " << endl;
    for(int i=0;i<row_num * row_ones_num;i++)
    {
            cout << "(" << parity_check_matrix[i][0] << "," << parity_check_matrix[i][1] << ")";
            cout << endl;
    }
    cout << "Bit Node Index: " << endl;
    for(int i=0;i<column_num;i++)
    {
        for (int j=0;j<column_ones_num;j++){
            cout << bit_node_index[i][j] << " ";
        }
        cout << endl;
    }


}

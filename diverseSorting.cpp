#include <iostream>
#include <conio.h>
#include <ctime>
#include <cmath>
#include <deque>
#include <set>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <string>
#define MAX_SZ_IDX 28
using namespace std;

class about_myself {
private:
	string name, ID, major, info, wish;
public:
	about_myself() {
		name = "姓名：xx  ";
		ID = "学号：12345678  ";
		major = "专业：material";
		info = "喜欢钻研，热爱编程，自学了不少计算机方面的知识，有从事计算机方面工作或研究的意愿";
		wish = "祝自己在下半年跨保/考研顺利，也祝老师身体健康，工作顺心！";
	}
	void print() {
		cout << name << ID << major << endl;
		cout << info << endl;
		cout << wish << endl;
	}
};

struct base_container {
	unsigned size;
	unsigned *begin, *end;
	base_container() {}
	base_container(const unsigned& N) : size(N) {
		begin = new unsigned[N];
		end = begin + N;
		for (unsigned* i = begin; i < end; i++) *i = rand();
	}
	base_container(const base_container& source) {
		size = source.size;
		begin = new unsigned[size];
		end = begin + size;
		for (unsigned i = 0; i < size; i++) *(begin + i) = source[i];
	}
	unsigned operator[](const unsigned& index) const {
		if (index < size) return begin[index];
		else return RAND_MAX + 1;
	}
	void print() {
		for (unsigned* i = begin; i < end; i++) cout << *i << ' ';
		cout << endl;
	}
	unsigned STL_sort() {
		time_t start, stop;
		start = clock();
		sort(begin, end);
		stop = clock();
		return stop - start;
	}
};

base_container all_nums_ori;

class container : public base_container {
private:	
	//以下两个函数，若对应节点不存在，则返回RAND_MAX + 1（32768）
	unsigned left_elem(unsigned idx) {
		return (*this)[idx * 2 + 1];
	}
	unsigned right_elem(unsigned idx) {
		return (*this)[idx * 2 + 2];
	}
	void heapify(unsigned idx) {	//调整堆化
		//建立最小值堆，若某节点的左右两子节点的值均不小于自己，则无需调整
		if ((*this)[idx] <= left_elem(idx) && (*this)[idx] <= right_elem(idx));
		else {	//不满足堆的性质，需要交换
			if (right_elem(idx) <= left_elem(idx)) {	
				//若右孩子不大于左孩子，则将右孩子与其父节点交换
				swap(begin[idx * 2 + 2], begin[idx]);
				heapify(idx * 2 + 2);					//递归地对右孩子的位置调整堆化
			}
			else {//若右孩子大于左孩子，则将左孩子与其父节点交换
				swap(begin[idx * 2 + 1], begin[idx]);
				heapify(idx * 2 + 1);					//递归地对左孩子的位置调整堆化
			}
		}
	}
	unsigned pop() {			//弹出堆顶元素
		unsigned res = *begin;  //保存堆顶元素
		*begin = RAND_MAX + 1;  //将堆顶元素置为32768（相当于让它不存在，那么在堆化时它会被压到堆底）
		heapify(0);				//从堆顶调整堆化
		return res;				//返回保存的值
	}
public:
	container() {}
	container(const base_container& source) : base_container(source) {}
	bool check() const {
		for (unsigned i = 0; i < size; i++)
			if (begin[i] != all_nums_ori.begin[i]) return false;
		return true;
	}
	unsigned runtime(void (container::*fp)(unsigned* , unsigned* , unsigned)) {
		time_t start, stop;
		start = clock();
		(this->*fp)(begin, end, 1);
		stop = clock();
		return stop - start;
	}
	void copy_to_set_sort(unsigned* , unsigned* , unsigned) {
		multiset<unsigned> set_container;
		for (unsigned* i = begin; i < end; i++) set_container.insert(*i);		
		unsigned* j = begin;
		for (auto num : set_container) *(j++) = num;
	}
	void insertion_sort(unsigned* b, unsigned* , unsigned interval) {
		for (unsigned* i = b; i < end - interval; i += interval)
			if (*i > *(i + interval)) {		//找到无序序列的第一个元素*(i + interval)
				unsigned* j = b;
				for (; *j <= *(i + interval); j += interval);	//找到该元素应插入的位置j
				unsigned temp = *(i + interval);				//保存待插入元素
				for (unsigned* t = i; t >= j; t -= interval)		//j及之后的有序序列元素后移
					*(t + interval) = *t;
				*j = temp;					//在j位置插入temp
			}
	}
	void bubble_sort(unsigned* , unsigned* , unsigned) {
		for (unsigned* i = end; i > begin; i--)
			for (unsigned* j = begin; j < i; j++)
				if (*j > *(j + 1)) swap(*j, *(j + 1));	
	}
	void selection_sort(unsigned* , unsigned* , unsigned) {
		for (unsigned* j = end; j > begin; j--) {
			unsigned* max = begin;
			for (unsigned* i = begin; i <= j; i++)		//寻找begin ~ j位置的最大值
				if (*i > *max) max = i;
			swap(*max, *j);								//将找到的最大值换到最后
		}
	}
	void shell_sort(unsigned* , unsigned* , unsigned) {
		//Sedgewick增量序列公式：9 * 4^i – 9 * 2^i + 1（i从0开始） 或 4^i – 3 * 2^i + 1（i从2开始）（^表示乘方）
		set<unsigned> Sedgewick_seq;	//增量序列需要按序排列，因此使用set存放
		unsigned elem = 1;
		//当某一个增量值不小于序列总长度的一半，结束计算
		for (unsigned i = 0; elem < size / 2; i++) {
			unsigned pow_2 = pow(2, i);
			elem = 9 * pow_2 * (pow_2 - 1) + 1;
			if (elem < size / 2) Sedgewick_seq.insert(elem);
		}
		for (unsigned i = 2; elem < size / 2; i++) {
			unsigned pow_2 = pow(2, i);
			elem = pow_2 * (pow_2 - 3) + 1;
			if (elem < size / 2) Sedgewick_seq.insert(elem);
		}
		//以上为Sedgewick增量序列的计算
		//set默认从小到大排列，因此需要逆序遍历Sedgewick序列，使用rbegin/rend逆序迭代器
		for (auto i = Sedgewick_seq.rbegin(); i != Sedgewick_seq.rend(); i++)
			for (unsigned j = 0; j < *i; j++)
				insertion_sort(begin + j, end, *i);
	}
	void radix_sort(unsigned* , unsigned* , unsigned) {
		deque<unsigned> buckets[10];		//根据基数分解待排正整数，按某一位上的值（0..9）将元素撒入十个桶中
		//待排正整数的取值范围为0 ~ 32767，最多有5位，i == 0 代表当前处理的是个位，将5位全部处理完毕则序列排序完毕
		for (unsigned i = 0; i < 5; i++) {  
			for (unsigned* p = begin; p < end; p++) {
				unsigned radix = unsigned(*p / pow(10, i)) % 10;	//取得基数
				buckets[radix].push_back(*p);						//撒入相应的桶中
			}
			unsigned* j = begin;
			for (unsigned k = 0; k < 10; k++)	//收集桶中数据，覆盖原序列
				for (; !buckets[k].empty(); j++) {
					*j = buckets[k].front();
					buckets[k].pop_front();
				}
		}
	}
	void bucket_sort(unsigned* , unsigned* , unsigned) {
		unsigned* buckets = new unsigned[RAND_MAX + 1]();		//申请32768个桶，每个桶中存放对应元素出现的次数
		for (unsigned* i = begin; i < end; i++) buckets[*i]++;  //统计各元素出现次数
		unsigned* temp_ptr = begin;
		for (unsigned i = 0; i < RAND_MAX + 1; i++)				//利用桶中的信息覆盖写入原容器
			for (unsigned j = 1; j <= buckets[i]; j++) 
				*(temp_ptr++) = i;
		delete[] buckets;
	}
	void quick_sort(unsigned* b, unsigned* e, unsigned) {
		//e指向待排序序列最后一个元素的后一位
		if (e - b == 2) {	//这意味着待排序序列长度为2
			if (*b > *(e - 1)) swap(*b, *(e - 1));
		}	
		else if (e - b == 1 || e - b == 0) return;
		//以上定义递归终点
		else {
			unsigned pivot = *(e - 1);		//选取最后一个元素为轴值
			unsigned* low = b; unsigned* high = e - 2;
			while (low < high) {
				for (; *low <= pivot && low < e - 1; low++);		//从low向右找到第一个比轴值大的元素
				for (; *high >= pivot && high > b; high--);		//从high向左找到第一个比轴值小的元素
				if (low < high) swap(*low, *high);				//若low、high没有错位，交换它们所指向的值
			}
			swap(*low, *(e - 1));			//将*low与序列最后一位的轴值交换
			quick_sort(b, low, 1);			//递归排左边
			quick_sort(low + 1, e, 1);		//递归排右边
		}
	}
	void merge_sort(unsigned* b, unsigned* e, unsigned) {
		if (e - b == 1) return; //e指向待排序序列最后一个元素的后一位
		else if (e - b == 2) {	//这意味着待排序序列长度为2
			if (*b > *(e - 1)) swap(*b, *(e - 1));
		} //以上定义递归终点
		else {
			merge_sort(b, b + (e - b) / 2, 1);	//递归排左边
			deque<unsigned>* left_array = new deque<unsigned>(b, b + (e - b) / 2);	//将排序结果存入一个deque中
			merge_sort(b + (e - b) / 2, e, 1);	//递归排右边
			deque<unsigned>* right_array = new deque<unsigned>(b + (e - b) / 2, e); //将排序结果存入一个deque中
			deque<unsigned>* merged_array = new deque<unsigned>;	//左右归并的结果放在这个deque中
			//比较左右两个deque的头元素，较小的那一个从它所在的deque中抛出并放入merged_array，直到某一个deque为空
			while (!left_array->empty() && !right_array->empty())
				if (left_array->front() < right_array->front()) {
					merged_array->push_back(left_array->front());
					left_array->pop_front();
				}
				else {
					merged_array->push_back(right_array->front());
					right_array->pop_front();
				}
			//将还未抛空的deque接到merged_array最后
			if (!left_array->empty()) merged_array->insert(merged_array->end(), left_array->begin(), left_array->end());
			if (!right_array->empty()) merged_array->insert(merged_array->end(), right_array->begin(), right_array->end());
			//用merged_array覆盖原序列
			unsigned* j = b; 
			for (auto num : *merged_array) *(j++) = num;
			delete left_array; delete right_array; delete merged_array;
		}
	}
	void heap_sort(unsigned* , unsigned*, unsigned) {
		container temp_array = *this;				//将待排序列拷贝出来
		unsigned last_parent_idx = (size - 2) / 2;  //找到最后一个非叶子节点的索引值
		for (int i = last_parent_idx; i >= 0; i--) temp_array.heapify(i);	//初次调整建堆
		//逐个抛出堆顶元素并填入原待排序列
		for (unsigned* i = begin; i < end; i++)	*i = temp_array.pop();		
	}
	void STL_heap_sort(unsigned* , unsigned* , unsigned) {
		make_heap(begin, end);
		sort_heap(begin, end);
	}
	void STL_merge_sort(unsigned* , unsigned* , unsigned) {
		stable_sort(begin, end);
	}
};


unsigned* results[13][MAX_SZ_IDX];	//第一层是算法（共13种），第二层是运行规模（共28个），第三层是运行次数（time次）
const unsigned data_size[] = { 
	100, 200, 300, 400, 500, 600, 700, 800, 900,
	1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000,
	10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000
};
const string alg_names[12] = {
	"插入   ", "冒泡   ", "选择   ", "希尔   ", "快速   ", 
	"堆     ", "归并   ", "基数   ", "桶     ", "set    ",
	"STL堆   ", "STL归并 "
};
void (container::*fp[12])(unsigned*, unsigned*, unsigned) = {
	&container::insertion_sort, &container::bubble_sort,
	&container::selection_sort, &container::shell_sort,
	&container::quick_sort, &container::heap_sort,
	&container::merge_sort, &container::radix_sort,
	&container::bucket_sort, &container::copy_to_set_sort,
	&container::STL_heap_sort, &container::STL_merge_sort
};

void run_once(unsigned sz_idx, unsigned t) {	//参数1指明要运行的数据规模，参数2指明当前运行的是第几次
	all_nums_ori = base_container(data_size[sz_idx]);	//构造待排序序列
	container all_nums[12];
	for (unsigned i = 0; i < 12; i++) all_nums[i] = base_container(all_nums_ori);	//拷贝12份
	results[0][sz_idx][t] = all_nums_ori.STL_sort();	//对原始数据运行STL_sort（内省排序）并计时
	for (unsigned i = 0; i < 12; i++)					//对拷贝出的12份数据运行另外12个排序函数并计时
		results[i + 1][sz_idx][t] = all_nums[i].runtime(fp[i]);
}

void run_once_print() {
	cout << endl << "请输入数据规模："; 
	unsigned N; cin >> N;
	cout << "初始化中...";
	all_nums_ori = base_container(N);			//构造待排序序列
	container all_nums[12];
	for (unsigned i = 0; i < 12; i++) all_nums[i] = base_container(all_nums_ori);	//拷贝12份
	cout << "初始化完毕！" << endl << endl;
	cout << "算法 耗时/ms 正确？" << endl;
	//输出STL_sort（内省排序）的运行时长
	cout << "STL    " << all_nums_ori.STL_sort() << ' '; cout << '1' << endl;
	for (unsigned i = 0; i < 12; i++) {
		//对拷贝出的12份数据运行另外12个排序函数，计时，输出
		cout << alg_names[i] << all_nums[i].runtime(fp[i]) << ' ';
		cout << all_nums[i].check() << endl;		//检查排序结果是否正确
	}
}


int main() {
	cout << "关于作者：" << endl;
	about_myself().print();
	srand(time(0));		//初始化随机数生成器
	cout << endl << "请输入运行模式：1 - 在指定的数据规模下将所有算法运行一遍，运行时间输出屏幕" << endl;
	cout << "2 - 在大范围数据规模下将所有算法运行指定的次数，在文件中输出运行时间的统计情况  其他 - 退出程序";
	char m = _getche(); cout << endl;
	switch (m) {
		case '1': run_once_print(); break;
		case '2': {
			cout << "请输入运行次数，程序将对 100 - 100000 的数据规模进行运行，在result.txt中输出各算法在不同数据规模上运行时间的平均值：";
			unsigned time; cin >> time;
			for (unsigned i = 0; i < 13; i++)	//results三维数组初始化
				for (unsigned j = 0; j < MAX_SZ_IDX; j++)
					results[i][j] = new unsigned[time];
			//对data_size中各大数据规模运行time次，耗时记录在results中
			for (unsigned i = 0; i < MAX_SZ_IDX; i++)
				for (unsigned j = 0; j < time; j++) {
					run_once(i, j);
					cout << "数据规模" << data_size[i] << "运行第" << j + 1 << "次结束！" << endl;
				}
			//在文件中输出统计结果
			ofstream file("result.txt");
			for (unsigned i = 0; i < 13; i++)
				for (unsigned j = 0; j < MAX_SZ_IDX; j++) {
					if (i == 0) file << "STL    ";
					else file << alg_names[i - 1];
					file << ' ' << data_size[j] << ' '
						<< accumulate(results[i][j], results[i][j] + time, 0) / time << endl;
					//accumulate函数对results[i][j][0]到results[i][j][time - 1]求和
					delete[] results[i][j];
				}
			file.close(); break;
		}
		default: break;
	}
	system("pause");
	return 0;
}

#include"filesystem.h"
#include<iostream>
#include<string>

using namespace std;

void FileSystem::writeFile() {
    fstream fs("D:\\sys.dat", ios::in | ios::out | ios::binary);
    //д�û�
    int len = 0;
    len = this->users.size();
    fs.write((char*)&len, sizeof(len));
    if (len != 0) {
        fs.write((char*)&users[0], sizeof(users[0]) * len);
    }
    //д���б�
    fs.seekg(200 * 100 + 4 + 1, ios::beg);
    len = this->free.size();
    fs.write((char*)&len, sizeof(len));
    fs.write((char*)&this->free[0], sizeof(free[0]) * len);
    //д����inode
    fs.seekg(20004 + 4 + 48770 * 4 + 1, ios::beg);
    len = this->fi.size();
    fs.write((char*)&len, sizeof(len));
    fs.write((char*)&this->fi[0], sizeof(fi[0]) * len);
    //дinode
    fs.seekg(4*1024*1024+1,ios::beg);
    len = this->inodes.size();
    fs.write((char*)&len, sizeof(len));
    fs.write((char*)&this->inodes[0], sizeof(inodes[0]) * len);
    fs.close();
}

void FileSystem::readFile() {
    ifstream fs("D:\\sys.dat", ios::binary | ios::in);
    //���û�
    int u_num;
    fs.read((char*)&u_num, sizeof(u_num));
    this->users = vector<User>(u_num);
    if (u_num != 0) {
        fs.read((char*)&users[0], sizeof(users[0]) * u_num);
    }
    //�����б�
    int f_num;
    fs.seekg(200 * 100 + 4 + 1, ios::beg);
    fs.read((char*)&f_num, sizeof(f_num));
    this->free = vector<int>(f_num);
    fs.read((char*)&free[0], sizeof(free[0]) * f_num);
    //������inode
    int fi_num;
    fs.seekg(20004 + 4 + 48770 * 4 + 1, ios::beg);
    fs.read((char*)&fi_num, sizeof(fi_num));
    this->fi = vector<int>(fi_num);
    fs.read((char*)&this->fi[0], sizeof(fi[0]) * fi_num);

    //��inode
    fs.seekg(4*1024*1024+1,ios::beg);
    int i_num;
    fs.read((char*)&i_num, sizeof(i_num));
    //cout << i_num << endl;
    this->inodes = vector<inode>(i_num);

    fs.read((char*)&this->inodes[0], sizeof(inodes[0]) * i_num);

    fs.close();
}

void FileSystem::regis() {
    //ע���û�
    string a, p;
    cout << "account:";
    cin >> a;
    //�鿴�Ƿ����д��û������û�
    int len = this->users.size();
    for (int i = 0; i < len; i++) {
        if (a == (string)this->users[i].acc) {
            cout << "���û����Ѵ���!" << endl;
            return; 
        }
    }
    cout << "password:";
    cin >> p;
    User u = User();
    len = p.length();
    for (int i = 0; i < len; i++)
        u.pass[i] = p[i];
    len = a.length();
    for (int i = 0; i < len; i++)
        u.acc[i] = a[i];
    this->users.push_back(u);
    //����inode
    inode ino = inode();
    for (int i = 0; i < len; i++) {
        ino.filename[i] = a[i];
        ino.owner[i] = a[i];
    }
    ino.block[0] = this->free[0];
    free.erase(free.begin());
    ino.id = this->fi[0];
    fi.erase(fi.begin());
    ino.prev = 0;
    ino.dir = 1;

    inodes.push_back(ino);
    writeFile();
    //ע���û���Ŀ¼
    /*int num;
    fstream fs;
    fs.open("D:\\sys.dat", ios::binary | ios::in | ios::out);
    fs.seekg(28*1024*1024+1,ios::beg);
    fs.read((char*)&num, sizeof(num));
    num += 1;
    cout << num << endl;
    fs.seekg(-4,ios::cur);
    fs.write((char*)&num,sizeof(num));
    fs.seekg(4 *(num-1), ios::cur);
    fs.write((char*)&ino.id, sizeof(ino.id));
    cout << ino.id << endl;

    fs.seekg(28*1024*1024+1+ino.id*12556,ios::beg);
    num = 0;
    fs.write((char*)&num,sizeof(num));
    fs.close();*/
    //mkdir(a);

}

string& ClearHeadTailSpace(string& str) {
    if (str.empty()) {
        return str;
    }
    str.erase(0, str.find_first_not_of(" "));
    str.erase(str.find_last_not_of(" ") + 1);
    return str;
}

void FileSystem::init() {
    //��ʼ�����еĿ��Ϊδʹ��
    for (int i = 0; i < 48770; i++)
    {
        this->free.push_back(i);
        this->fi.push_back(i);
    }
    //д��һ��/Ŀ¼
    fstream fs;
    fs.open("D:\\sys.dat", ios::binary | ios::out | ios::in);
    inode root;
    root.id = 0; root.block[0] = 0; root.prev = 0; root.filename[0] = '/'; root.owner[0] = '/'; root.dir = 1;
    fi.erase(fi.begin());
    free.erase(free.begin());
    this->inodes.push_back(root);
    fs.seekg(28*1024*1024+1,ios::beg);
    int n = 0;
    fs.write((char*)&n, sizeof(n));
    fs.close();
    this->writeFile();
}

void FileSystem::login() {
    char a[50],p[50];
    cout << "account:";
    cin >> a;
    cout << "password:";
    cin >> p;
    int len = this->users.size();
    for (int i = 0; i < len; i++) {
        //cout << this->users[i].acc << " " << this->users[i].pass << endl;
        if (strcmp(a,this->users[i].acc) == 0 && strcmp(p, this->users[i].pass) == 0) {
            this->user = a;
            int l = this->inodes.size();
            for (int i = 0; i < l; i++) {
                if (strcmp(a, inodes[i].filename) == 0 && this->inodes[i].prev == 0) {
                    this->curInode = inodes[i].id;
                    this->path = inodes[i].filename;
                }
            }
            return;
        }
    }
    cout << "�˻����������!" << endl;
}

void FileSystem::mkdir(string dir) {
    int len = this->inodes.size();
    for (int i = 0; i < len; i++) {
        if ((string)this->inodes[i].filename == dir && this->inodes[i].prev == this->curInode) {
            cout << "��Ŀ¼���ѱ�ʹ��!" << endl;
            return;
        }
    }
    inode ino = inode();
    len = dir.size();
    for (int i = 0; i < len; i++)
        ino.filename[i] = dir[i];
    len = this->user.size();
    for (int i = 0; i < len; i++)
        ino.owner[i] = this->user[i];
    ino.block[0] = this->free[0];
    free.erase(free.begin());
    ino.id = fi[0];
    fi.erase(fi.begin());
    ino.prev = this->curInode;
    ino.dir = 1;
    inodes.push_back(ino);

    fstream fs;
    fs.open("D:\\sys.dat", ios::binary | ios::out | ios::in);
    //д��ǰinode��block
    fs.seekg(28 * 1024 * 1024 + 1 + ino.block[0] * 12556, ios::beg);
    int n = 0;
    fs.write((char*)&n, sizeof(n));
    
    //д�ϼ�inode��block
    fs.seekg(28 * 1024 * 1024 + 1 + ino.prev * 12556, ios::beg);
    int inum;
    fs.read((char*)&inum,sizeof(inum));
    //cout << inum << endl;
    inum++;
    fs.seekp(-4);
    fs.write((char*)&inum,sizeof(inum));
    fs.seekp(4*(inum-1));
    fs.write((char*)&ino.id,sizeof(ino.id));
    fs.close();
    //������inodeд���ļ�
    writeFile();
    /*fs.open("D:\\sys.dat", ios::binary | ios::out | ios::in);
    fs.seekg(4*1024*1024+1,ios::beg);
    int num;
    cout << fs.tellp() << endl;
    fs.read((char*)&num, sizeof(num));
    cout << fs.tellp() << endl;
    num += 1;
    fs.seekg(-4, ios::cur);
    cout << fs.tellp() << endl;
    fs.write((char*)&num,sizeof(num));
    for (int i = 0; i < num; i++) {
        fs.write((char*)&this->inodes[i], sizeof(inodes[i]));
    }
    fs.close();*/
}

void FileSystem::create(string file) {
    int len = this->inodes.size();
    for (int i = 0; i < len; i++) {
        if ((string)this->inodes[i].filename == file && this->inodes[i].prev == this->curInode) {
            cout << "���ļ����ѱ�ʹ��!" << endl;
            return;
        }
    }
    inode ino = inode();
    len = file.size();
    for (int i = 0; i < len; i++)
        ino.filename[i] = file[i];
    len = this->user.size();
    for (int i = 0; i < len; i++)
        ino.owner[i] = this->user[i];
    ino.block[0] = this->free[0];
    free.erase(free.begin());
    ino.id = fi[0];
    fi.erase(fi.begin());
    ino.prev = this->curInode;
    ino.dir = 0;
    inodes.push_back(ino);

    //д�ϼ�inode��block
    fstream fs;
    fs.open("D:\\sys.dat", ios::binary | ios::in | ios::out);
    fs.seekg(28 * 1024 * 1024 + 1 + ino.prev * 12556, ios::beg);
    int inum;
    fs.read((char*)&inum, sizeof(inum));
    inum++;
    fs.seekp(-4);
    fs.write((char*)&inum, sizeof(inum));
    fs.seekp(4 * (inum - 1));
    fs.write((char*)&ino.id, sizeof(ino.id));
    fs.close();

    //
    writeFile();
}

void FileSystem::dir() {
    readFile();
    /*fstream fs;
    fs.open("D:\\sys.dat", ios::in | ios::binary);
    fs.seekg(28 * 1024 * 1024 + 1 + this->curInode*12556, ios::beg);
    int num;
    fs.read((char*)&num, sizeof(num));
    vector<int> inos;
    for (int i = 0; i < num; i++) {
        int t;
        fs.read((char*)&t, sizeof(t));
        inos.push_back(t);
    }
    num = this->inodes.size();
    int l = inos.size();
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < l; j++) {
            if (this->inodes[i].id == inos[i]) {
                if (this->inodes[i].dir == 1) {
                    cout << "d " << this->inodes[i].filename << endl;
                } else {
                    cout <<"- " << this->inodes[i].filename << endl;
                }
            }
        }
    }*/
    int len = this->inodes.size();
    for (int i = 0; i < len; i++) {
        if (this->inodes[i].prev == this->curInode) {
            if (this->inodes[i].dir == 1) {
                cout << "<DIR> " << this->inodes[i].filename << endl;
            }
            else {
                cout << "<FIL> " << this->inodes[i].filename << endl;
            }
        }
    }
}

void FileSystem::cd(string dir) { //�е�С����
    readFile();
    int len = this->inodes.size();
    if (dir == "..") {
        int id = -1;
        for (int i = 0; i < len; i++) {
            if (this->curInode == this->inodes[i].id) {
                id = i;
                break;
            }
        }
        if (this->inodes[id].prev != 0) {
            for (int i = 0; i < len; i++) {
                if (this->inodes[i].id == this->inodes[id].prev) {
                    int index = this->path.find(this->inodes[id].filename);
                    this->path = this->path.substr(0, index - 1);
                    this->curInode = this->inodes[i].id;
                    return;
                }
            }
        }
        else {
            
        }
    }
    else if (dir == "/") {
        this->curInode = 0;
        this->path = "/";
    }
    else {
        char name[50] = { 0 };
        int l = dir.size();
        int jud = 0;
        for (int i = 0; i < l; i++) {
            name[i] = dir[i];
        }
        for (int i = 0; i < len; i++) {
            if (this->inodes[i].prev == this->curInode && strcmp(this->inodes[i].filename, name) == 0 && this->inodes[i].dir == 1) {
                this->curInode = this->inodes[i].id;
                string str = this->inodes[i].filename;
                this->path += "/" + str;
                return;
            }
        }
        cout << "�޴�Ŀ¼!" << endl;
    }
}

void FileSystem::rmdir(string dir) {
    readFile();
    int len = this->inodes.size();
    int id=-1;
    for (int i = 0; i < len; i++) {
        if (this->inodes[i].prev == this->curInode && (string)this->inodes[i].filename == dir && this->inodes[i].dir == 1) {
            id = this->inodes[i].id;
            for (int j = 0; j < len; j++) {
                if (this->inodes[j].prev == id) {
                    cout << "��Ŀ¼��Ϊ��!" << endl;
                    return;
                }
            }
        }
    }
    if (id != -1) {
        /*cout << id << " " << id_prev << endl;
        fstream fs;
        fs.open("D:\\sys.dat", ios::binary | ios::in | ios::out);
        fs.seekg(28 * 1024 * 1024 + 1 + id_prev * 12556, ios::beg);
        int num;
        fs.read((char*)&num, sizeof(num));
        cout << num << endl;*/
        this->inodes.erase(inodes.begin()+id);
        free.insert(free.begin(), id);
        fi.insert(fi.begin(), id);

        //
        writeFile();
    }
    else {
        cout << "��Ŀ¼������!" << endl;
    }
}

void FileSystem::del(string file) {
    readFile();
    int len = this->inodes.size();
    for (int i = 0; i < len; i++) {
        if (this->inodes[i].prev == this->curInode && (string)this->inodes[i].filename == file && this->inodes[i].dir == 0) {
            fstream fs;
            fs.open("D:\\sys.dat", ios::binary | ios::in | ios::out);
            fs.seekg(28 * 1024 * 1024 + 1 + this->inodes[i].block[0] * 12556, ios::beg);
            char content[12556] = { 0 };
            fs.read((char*)&content, sizeof(content));
            fs.close();
            //cout << num << endl;
            this->inodes.erase(inodes.begin() + i);
            free.insert(free.begin(), i);
            fi.insert(fi.begin(), i);

            //
            writeFile();
            return;
        }
    }
    cout << "���ļ�������!" << endl;
}

void FileSystem::find(string file) {
    readFile();
    int len = this->inodes.size();
    vector<string> v;
    int cnt = -1;
    for (int i = 0; i < len; i++) {
        if ((string)this->inodes[i].filename == file) {
            cnt++;
            v.push_back(file);
            int t = this->inodes[i].prev;
            for (int j = 0; j < len; j++) {
                if (this->inodes[j].id == t) {
                    t = j;
                }
            }
            while (t != 0) {
                v[cnt].insert(0,(string)this->inodes[t].filename+"/");
                t = this->inodes[t].prev;
                for (int j = 0; j < len; j++) {
                    if (this->inodes[j].id == t) {
                        t = j;
                    }
                }
            }
        }
    }
    for (int i = 0; i < cnt+1; i++) {
        cout << v[i] << endl;
    }
}

void FileSystem::move(string file,string dir) {
    readFile();
    //�ж��Ƿ���Ŀ¼
    int len = this->inodes.size();
    int id = -1;
    for (int i = 0; i < len; i++) {
        if (this->inodes[i].prev == this->curInode && (string)this->inodes[i].filename == file && this->inodes[i].dir == 0) {
            id = i;
            break;
        }
    }
    if (id == -1) {
        cout << "���ļ�������!" << endl;
    }
    else {
        int index = 0;
        vector<int> ind;
        vector<string> f;
        ind.push_back(-1);
        while ((index = dir.find("/", index)) != string::npos) {
            ind.push_back(index);
            index++;
        }
        len = ind.size();
        for (int i = 0; i < len - 1; i++) {
            f.push_back(dir.substr(ind[i] + 1, ind[i + 1] - ind[i] - 1));
            //cout << dir.substr(ind[i] + 1, ind[i + 1] - ind[i] - 1) << endl;
        }
        f.push_back(dir.substr(ind[len - 1] + 1, dir.length() - ind[len - 1] - 1));
        //cout << dir.substr(ind[len - 1] + 1, ind[len - 1] - ind[len - 1] - 1) << endl;
        len = this->inodes.size();
        index = f.size();
        int prev = 0;
        int i = 0, jud = 0;
        for (i = 0; i < index; i++) {
            jud = 0;
            for (int j = 0; j < len; j++) {
                if ((string)this->inodes[j].filename == f[i] && this->inodes[j].prev == prev && this->inodes[i].dir == 1) {
                    prev = this->inodes[j].id;
                    jud = 1;
                    break;
                }
            }
        }
        if (jud == 1 && i == index) {
            //�ж�Ŀ�ĵ�ַ�Ƿ���ͬ���ļ�
            for (i = 0; i < len; i++) {
                if (this->inodes[i].prev == prev && (string)this->inodes[i].filename == file) {
                    jud = 0;
                    break;
                }
            }
            if (jud == 0) {
                cout << "��Ŀ¼�Ѵ��ڴ��ļ�!" << endl;
            }
            //�ƶ�
            else {
                this->inodes[id].prev = prev;
                writeFile();
            }
        }
        else {
            cout << "�޴�Ŀ¼!" << endl;
        }
    }
}

void FileSystem::copy(string file, string dir) {
    readFile();
    //�ж��Ƿ���Ŀ¼
    int len = this->inodes.size();
    int id = -1;
    for (int i = 0; i < len; i++) {
        if (this->inodes[i].prev == this->curInode && (string)this->inodes[i].filename == file && this->inodes[i].dir == 0) {
            id = this->inodes[i].id;
            break;
        }
    }
    if (id == -1) {
        cout << "���ļ�������!" << endl;
    }
    else {
        int index = 0;
        vector<int> ind;
        vector<string> f;
        ind.push_back(-1);
        while ((index = dir.find("/", index)) != string::npos) {
            ind.push_back(index);
            index++;
        }
        len = ind.size();
        for (int i = 0; i < len - 1; i++) {
            f.push_back(dir.substr(ind[i] + 1, ind[i + 1] - ind[i] - 1));
            //cout << dir.substr(ind[i] + 1, ind[i + 1] - ind[i] - 1) << endl;
        }
        f.push_back(dir.substr(ind[len - 1] + 1, dir.length() - ind[len - 1] - 1));
        //cout << dir.substr(ind[len - 1] + 1, ind[len - 1] - ind[len - 1] - 1) << endl;
        len = this->inodes.size();
        index = f.size();
        int prev = 0;
        int i = 0, jud = 0;
        for (i = 0; i < index; i++) {
            jud = 0;
            for (int j = 0; j < len; j++) {
                if ((string)this->inodes[j].filename == f[i] && this->inodes[j].prev == prev && this->inodes[i].dir == 1) {
                    prev = this->inodes[j].id;
                    jud = 1;
                    break;
                }
            }
        }
        if (jud == 1 && i == index) {
            //�ж�Ŀ�ĵ�ַ�Ƿ�����ͬ���ļ�
            for (i = 0; i < len; i++) {
                if (this->inodes[i].prev == prev && (string)this->inodes[i].filename == file) {
                    jud = 0;
                    break;
                }
            }
            if (jud == 0) {
                cout << "��Ŀ¼�Ѵ��ڴ��ļ�!" << endl;
            }
            //�����ļ�
            else {
                inode ino = inode();
                len = file.size();
                for (int i = 0; i < len; i++)
                    ino.filename[i] = file[i];
                len = this->user.size();
                for (int i = 0; i < len; i++)
                    ino.owner[i] = this->user[i];
                ino.block[0] = this->free[0];
                free.erase(free.begin());
                ino.id = fi[0];
                fi.erase(fi.begin());
                ino.prev = prev;
                ino.dir = 0;
                inodes.push_back(ino);

                //���ļ����ݸ��ƹ���
                char content[12556] = { 0 };
                fstream fs;
                fs.open("D:\\sys.dat", ios::binary | ios::in | ios::out);
                fs.seekg(28 * 1024 * 1024 + 1 + 12556 * id, ios::beg);
                fs.read((char*)&content, sizeof(content));
                cout << content << endl;
                fs.seekg(28 * 1024 * 1024 + 1 + 12556 * ino.id, ios::beg);
                fs.write((char*)&content, sizeof(content));
                fs.close();

                //
                writeFile();
            }
        }
        else {
            cout << "�޴�Ŀ¼!" << endl;
        }
    }
}

void FileSystem::open(string file) {
    readFile();
    if (this->curFile == -1) {
        int len = this->inodes.size();
        for (int i = 0; i < len; i++) {
            if ((string)this->inodes[i].filename == file && this->inodes[i].prev == this->curInode && this->inodes[i].dir == 0) {
                this->curFile = this->inodes[i].id;
                return;
            }
        }
        cout << "δ�ҵ����ļ�!" << endl;
    }
    else {
        cout << "�����ļ����򿪣����ȹرպ��ٴ�!" << endl;
    }
}

void FileSystem::read() {
    int len = this->inodes.size();
    for (int i = 0; i < len; i++) {
        if (this->inodes[i].id == this->curFile) {
            fstream fs;
            fs.open("D:\\sys.dat",ios::binary | ios::in);
            fs.seekg(28 * 1024 * 1024 + 1 + 12556 * this->inodes[i].block[0], ios::beg);
            char content[12556] = { 0 };
            fs.read((char*)&content, sizeof(content));
            fs.close();
            cout << content;
            return;
        }
    }
}

void FileSystem::write() {
    int len = this->inodes.size();
    for (int i = 0; i < len; i++) {
        if (this->inodes[i].id == this->curFile) {
            fstream fs;
            fs.open("D:\\sys.dat", ios::binary | ios::in | ios::out);
            fs.seekg(28 * 1024 * 1024 + 1 + 12556 * this->inodes[i].block[0], ios::beg);
            string content;
            cout << "�������ļ�����,��#fin����"<<endl;
            while (cin>>content && content != "#fin")
            {
                content += "\n";
                int len = content.size();
                char con[12556] = { 0 };
                for (int i = 0; i < len; i++) {
                    con[i] = content[i];
                }
                //cout << con;
                fs.write((char*)&con, len);
                //cout<<fs.tellp();
            }
            fs.close();
            return;
        }
    }
}

void FileSystem::close() {
    if (this->curFile != -1) {
        this->curFile = -1;
    }
    else {
        cout << "δ���ļ�!" << endl;
    }
}

void FileSystem::import(string dir, string file) {
    readFile();
    fstream fs;
    fs.open(dir, ios::binary | ios::out | ios::in);
    char content[12556] = { 0 };
    fs.read((char*)&content,sizeof(content));
    fs.close();
    create(file);
    fs.open("D:\\sys.dat", ios::binary | ios::out | ios::in);
    int len = this->inodes.size();
    for (int i = 0; i < len; i++) {
        if (this->inodes[i].prev == this->curInode && (string)this->inodes[i].filename == file) {
            fs.seekg(28 * 1024 * 1024 + 1 + 12556 * this->inodes[i].id, ios::beg);
            fs.write((char*)&content, sizeof(content));
            fs.close();

            writeFile();
            return;
        }
    }
}

void FileSystem::exp(string file, string dir) {
    readFile();
    int len = this->inodes.size();
    for (int i = 0; i < len; i++) {
        if (this->inodes[i].prev == this->curInode && (string)this->inodes[i].filename == file) {
            char content[12556] = { 0 };
            fstream fs;
            fs.open("D:\\sys.dat", ios::binary | ios::out | ios::in);
            fs.seekg(28 * 1024 * 1024 + 1 + 12556 * this->inodes[i].block[0], ios::beg);
            fs.read((char*)&content,sizeof(content));
            fs.close();
            //cout << content << endl;
            //�����ļ�
            ofstream MyFile(dir);
            MyFile.close();
            //д���ļ�
            fs.open(dir, ios::binary | ios::out | ios::in);
            fs.write((char*)&content,sizeof(content));
            fs.close();
            return;
        }
    }
    cout << "δ�ҵ����ļ�!" << endl;
}

void FileSystem::head(string file, int num) {
    readFile();
    int len = this->inodes.size();
    for (int i = 0; i < len; i++) {
        if (this->inodes[i].prev == this->curInode && (string)this->inodes[i].filename == file) {
            char content[12556] = { 0 };
            fstream fs;
            fs.open("D:\\sys.dat", ios::binary | ios::in);
            fs.seekg(28 * 1024 * 1024 + 1 + 12556 * this->inodes[i].id, ios::beg);
            fs.read((char*)&content,sizeof(content));
            fs.close();
            vector<int> index;
            for (int i = 0; i < 12556; i++) {
                if (content[i] == '\n') {
                    index.push_back(i);
                }
                else if (content[i] == '\0') {
                    break;
                }
            }
            len = index.size();
            if (num >= len) {
                cout << content;
            }
            else {
                //cout << index[num] << endl;
                for (int i = 0; i < index[num - 1] + 1; i++) {
                    cout << content[i];
                }
            }
            return;
        }
    }
    cout << "δ�ҵ����ļ�!" << endl;
}

void FileSystem::tail(string file, int num) {
    readFile();
    int len = this->inodes.size();
    for (int i = 0; i < len; i++) {
        if (this->inodes[i].prev == this->curInode && (string)this->inodes[i].filename == file) {
            char content[12556] = { 0 };
            fstream fs;
            fs.open("D:\\sys.dat", ios::binary | ios::in);
            fs.seekg(28 * 1024 * 1024 + 1 + 12556 * this->inodes[i].id, ios::beg);
            fs.read((char*)&content, sizeof(content));
            fs.close();
            vector<int> index;
            for (int i = 0; i < 12556; i++) {
                if (content[i] == '\n') {
                    index.push_back(i);
                }
                else if (content[i] == '\0') {
                    break;
                }
            }
            len = index.size();
            if (num >= len) {
                cout << content;
            }
            else {
                //cout << index[num] << endl;
                for (int i = index[len-num-1]+1; i < index[len - 1] + 1; i++) {
                    cout << content[i];
                }
            }
            return;
        }
    }
    cout << "δ�ҵ����ļ�!" << endl;
}

void FileSystem::show() {
    string command;
    while (true) {
        if (this->user != "") {
            cout << "[" << this->user << "@" << this->path << "]>";
        }
        else {
            cout << ">";
        }
        getline(cin, command);
        ClearHeadTailSpace(command); //ȥ����ͷ��β�Ŀո�
        if (command == "") {
            getline(cin, command);
            ClearHeadTailSpace(command); //ȥ����ͷ��β�Ŀո�
        }
        int index = command.find(' ');
        if (index == -1) {
            if (command == "exit") {
                this->writeFile();
                break;
            }
            else if (command == "login") {
                login();
            }
            else if (command == "regis") {
                regis();
            }
            else if (command == "dir") {
                dir();
            }
            else if (command == "clear") {
                system("cls");
            }
            else if (command == "close") {
                close();
            }
            else if (command == "read") {
                read();
            }
            else if (command == "write") {
                write();
            }
            else if (command == "help") {
                help();
            }
            else {
                cout << "ָ������ȱ�ٲ���!" << endl;
            }
        }
        else {
            if (this->user == "") {
                cout << "���ȵ�¼ϵͳ!" << endl;
            }
            else {
                if (command.substr(0, index) == "delete") {
                    string file = command.substr(index + 1, command.length() - index);
                    del(file);
                }
                else if (command.substr(0, index) == "cd") {
                    string dir = command.substr(index + 1, command.length() - index);
                    cd(dir);
                }
                else if (command.substr(0, index) == "open") {
                    string file = command.substr(index + 1, command.length() - index);
                    open(file);
                }
                else if (command.substr(0, index) == "move") {
                    int index1 = command.find(" ",index+1);
                    string file = command.substr(index + 1, index1 - index-1);
                    string dir = command.substr(index1 + 1, command.length() - index1);
                    //cout << file << " / " << dir << endl;
                    move(file,dir);
                }
                else if (command.substr(0, index) == "copy") {
                    int index1 = command.find(" ", index + 1);
                    string file = command.substr(index + 1, index1 - index - 1);
                    string dir = command.substr(index1 + 1, command.length() - index1);
                    //cout << file << "/ " << dir << endl;
                    copy(file, dir);
                }
                else if (command.substr(0, index) == "find") {
                    string file = command.substr(index + 1, command.length() - index);
                    //cout << file << "/" << endl;
                    find(file);
                }
                else if (command.substr(0, index) == "mkdir") {
                    string file = command.substr(index + 1, command.length() - index);
                    //cout << file <<"/" << endl;
                    mkdir(file);
                }
                else if (command.substr(0, index) == "create") {
                    string dir = command.substr(index + 1, command.length() - index);
                    //cout << dir <<"/" << endl;
                    create(dir);
                }
                else if (command.substr(0, index) == "rmdir") {
                    string dir = command.substr(index + 1, command.length() - index);
                    rmdir(dir);
                }
                else if (command.substr(0, index) == "import") {
                    int index1 = command.find(" ", index + 1);
                    string dir = command.substr(index + 1, index1 - index - 1);
                    string file = command.substr(index1 + 1, command.length() - index1);
                    //cout << dir << "/ " << file << endl;
                    import(dir, file);
                }
                else if (command.substr(0, index) == "export") {
                    int index1 = command.find(" ", index + 1);
                    string file = command.substr(index + 1, index1 - index - 1);
                    string dir = command.substr(index1 + 1, command.length() - index1);
                    //cout << file << " / " << dir << endl;
                    exp(file, dir);
                }
                else if (command.substr(0, index) == "head") {
                    int index1 = command.find(" ", index + 1);
                    int num = stoi(command.substr(index + 2, index1 - index - 1));
                    string file = command.substr(index1 + 1, command.length() - index1);
                    //cout <<file <<" " << num << "x" << endl;
                    head(file, num);
                }
                else if (command.substr(0, index) == "tail") {
                    int index1 = command.find(" ", index + 1);
                    int num = stoi(command.substr(index + 2, index1 - index - 1));
                    string file = command.substr(index1 + 1, command.length() - index1);
                    //cout << file << " " << num << "x" << endl;
                    tail(file, num);
                }
                else {
                    cout << "ָ�����!" << endl;
                }
            }
        }
    }
}

void FileSystem::welcome() {
    cout << "��ӭʹ�������ļ�����ϵͳ\n";
    cout << "help----------�鿴����\n";
}

void FileSystem::help() {
    cout << "login--------��¼ϵͳ\n";
    cout << "regis--------ע���û�\n";
    cout << "cd-----------�л�Ŀ¼\n";
    cout << "create-------�����ļ�\n";
    cout << "delete-------ɾ���ļ�\n";
    cout << "open---------���ļ�\n";
    cout << "close--------�ر��ļ�\n";
    cout << "move---------�ƶ��ļ�\n";
    cout << "copy---------�����ļ�\n";
    cout << "find---------�����ļ�\n";
    cout << "mkdir--------������Ŀ¼\n";
    cout << "rmdir--------ɾ����Ŀ¼\n";
    cout << "read---------���ļ���������\n";
    cout << "write--------���ļ�д������\n";
    cout << "dir----------�鿴��ǰĿ¼�ļ�\n";
    cout << "head---------�鿴�ļ�ǰn������\n";
    cout << "tail---------�鿴�ļ���n������\n";
    cout << "import-------�ӱ��ش��̵����ļ�\n";
    cout << "export-------������ϵͳ�����ļ�\n";
}
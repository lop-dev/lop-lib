using System.Collections;

namespace UDLib.Utility
{
    /// <summary>
    /// 单向链表结点类
    /// </summary>
    /// <typeparam name="T">泛型</typeparam>
    public class LinkNode<T>
    {
        private T data;             //数据域，当前节点数据
        private LinkNode<T> next;   //引用域，下一结点

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="item">数据域</param>
        /// <param name="node">引用域</param>
        public LinkNode(T item, LinkNode<T> p)
        {
            data = item;
            next = p;
        }

        /// <summary>
        /// 构造函数，头结点
        /// </summary>
        /// <param name="node">引用域</param>
        public LinkNode(LinkNode<T> p)
        {
            next = p;
        }

        /// <summary>
        /// 构造函数，尾结点
        /// </summary>
        /// <param name="item">数据域</param>
        public LinkNode(T item)
        {
            data = item;
            next = null;
        }

        /// <summary>
        /// 构造函数，无参数
        /// </summary>
        public LinkNode()
        {
            data = default(T);
            next = null;
        }

        /// <summary>
        /// 数据域
        /// </summary>
        public T Data
        {
            get { return data; }
            set { data = value; }
        }

        /// <summary>
        /// 引用域
        /// </summary>
        public LinkNode<T> Next
        {
            get { return next; }
            set { next = value; }
        }
    }

    /// <summary>
    /// 链表类，包含链表定义和基本操作
    /// </summary>
    public class CLinkList<T>
    {
        private LinkNode<T> head;   //单链表的头结点

        /// <summary>
        /// 头结点属性
        /// </summary>
        public LinkNode<T> Head
        {
            get { return head; }
            set { head = value; }
        }

        /// <summary>
        /// 构造函数
        /// </summary>
        public CLinkList()
        {
            head = null;
        }

        /// <summary>
        /// 求单链表长度
        /// </summary>
        /// <returns></returns>
        public int GetLength()
        {
            LinkNode<T> p = head;
            int len = 0;
            while (p != null)
            {
                ++len;
                p = p.Next;
            }
            return len;
        }

        /// <summary>
        /// 清空链表
        /// </summary>
        public void Clear()
        {
            head = null;
        }

        /// <summary>
        /// 判断链表是否为空
        /// </summary>
        /// <returns></returns>
        public bool IsEmpty()
        {
            if (head == null)
                return true;
            else
                return false;
        }

        /// <summary>
        /// 在链表末尾添加新元素
        /// </summary>
        /// <param name="item">新结点</param>
        public void Append(T item)
        {
            LinkNode<T> q = new LinkNode<T>(item);
            if (head == null)
            {
                head = q;
                return;
            }

            LinkNode<T> p = head;
            while (p.Next != null)
            {
                p = p.Next;
            }
            p.Next = q;
        }

        /// <summary>
        /// 在第i个结点的位置前插入一个结点
        /// </summary>
        /// <param name="item">新结点</param>
        /// <param name="i">插入位置</param>
        public bool Insert(T item, int i)
        {
            if (IsEmpty() || i < 1 || i > GetLength())
            {
                return false;
            }

            LinkNode<T> q = new LinkNode<T>(item);
            if (i == 1)
            {
                q.Next = head;
                head = q;
                return true;
            }

            LinkNode<T> p = head;
            LinkNode<T> r = new LinkNode<T>();
            int j = 1;
            while (p.Next != null && j < i)
            {
                r = p;
                p = p.Next;
                ++j;
            }

            if (j == i)
            {
                q.Next = p;
                r.Next = q;
            }
            return true;
        }

        /// <summary>
        /// 删除链表第i个结点
        /// </summary>
        /// <param name="i">删除位置</param>
        /// <returns></returns>
        public T Delete(int i)
        {
            if (IsEmpty() || i < 1 || i > GetLength())
            {
                return default(T);
            }

            LinkNode<T> q = new LinkNode<T>();
            if (i == 1)
            {
                q = head;
                head = head.Next;
                return q.Data;
            }

            LinkNode<T> p = head;
            int j = 1;
            while (p.Next != null && j < i)
            {
                q = p;
                p = p.Next;
                ++j;
            }

            if (j == i)
            {
                q.Next = p.Next;
                return p.Data;
            }

            return default(T);
        }

        /// <summary>
        /// 获得链表第i个数据
        /// </summary>
        /// <param name="i"></param>
        /// <returns></returns>
        public T GetElem(int i)
        {
            if (IsEmpty() || i < 1)
            {
                return default(T);
            }

            LinkNode<T> p = head;
            int j = 1;
            while (p.Next != null && j < i)
            {
                p = p.Next;
                ++j;
            }

            if (j == i)
            {
                return p.Data;
            }

            return default(T);
        }

        /// <summary>
        /// 查找链表中值为value的结点
        /// </summary>
        /// <param name="value">查找的值</param>
        /// <returns>-1无效值</returns>
        public int Locate(T value)
        {
            if (IsEmpty())
            {
                return -1;
            }

            LinkNode<T> p = head;
            int i = 1;
            while (!p.Data.Equals(value) && p.Next != null)
            {
                p = p.Next;
                ++i;
            }

            if (i > GetLength())
            {
                return -1;
            }

            return i;
        }
    }
}
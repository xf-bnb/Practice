package Frank;

// 一个简单的栈实现
public class SimpleStack {

    private int[] stack;
    private int index;

    public SimpleStack(int n) {
        this.index = 0;
        this.stack = new int[n];
    }

    public void push(int value) {
        stack[index++] = value;
    }

    public int pop() {
        return stack[--index];
    }

    public boolean isFull() {
        return (index == stack.length);
    }

    public boolean isEmpty() {
        return (index == 0);
    }

    public int[] getStack() {
        return stack;
    }

    public int getIndex() {
        return index;
    }
}

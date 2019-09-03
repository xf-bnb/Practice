package Frank;

public class EightQueens {

    // 对n个元素进行全排列
    public static int Permutation(int n) {
        int count = 0;
        SimpleStack stack = new SimpleStack(n);

        for (int pos = 0; ;) {
            if (PushStack(stack, pos, n)) {
                if (stack.isFull()) {
                    Show(stack.getStack());
                    ++count;

                    pos = stack.pop() + 1;
                }
                else {
                    pos = 0;
                }
            }
            else {
                if (stack.isEmpty())
                    break;
                else
                    pos = stack.pop() + 1;
            }
        }

        return count;
    }

    // 从start到end之间查找满足检查函数CheckMap的数字添加到stack中，如果找到则添加并返回true，否则返回false。
    static boolean PushStack(SimpleStack stack, int start, int end) {
        for (; start < end; ++start) {
            if (CheckMap(stack, start)) {
                stack.push(start);
                return true;
            }
        }

        return false;
    }

    // 对于盘面stack，判定是否可以在x位置放置棋子
    static boolean CheckMap(final SimpleStack stack, int x) {
        for (int i = 0; i < stack.getIndex(); ++i) {
            int v = stack.getStack()[i];
            if (v == x || (stack.getIndex() - i) == Math.abs(v - x)) {
                return false;
            }
        }

        return true;
    }

    // 打印一个盘面
    public static void Show(final int[] map) {
        for (int x : map) {
            for (int i = 0; i < map.length; ++i) {
                if (i == x) {
                    System.out.print("* ");
                } else {
                    System.out.print("- ");
                }
            }

            System.out.println();
        }

        System.out.println("====================");
    }

    public static void main(String[] args) {
        int n = Permutation(8);

        System.out.println("map count: " + n);
    }

}

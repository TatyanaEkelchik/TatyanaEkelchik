#include <stdio.h>
#include <stdbool.h>

typedef struct
    {
        int arr_h, arr_m, depp_h, depp_m;
    }train;

bool change(train from, train to);

int main(void)
{    
    train a, b, c;

    printf("Train A arrival time:\n");
    int n_1 = scanf("%d:%d", &a.arr_h, &a.arr_m);
    if (a.arr_h > 23 || a.arr_m > 59 || n_1 < 2 || a.arr_h < 0 || a.arr_m < 0 )
    {
        printf("Invalid input.\n");
        return 0;
    }
    printf("Train A departure time:\n");
    int n_2 = scanf("%d:%d", &a.depp_h, &a.depp_m);
    if (a.depp_h > 23 || a.depp_m > 59 || n_2 < 2 || a.depp_h < 0 || a.depp_m < 0)
    {
        printf("Invalid input.\n");
        return 0;
    }

    printf("Train B arrival time:\n");
    int n_3 = scanf("%d:%d", &b.arr_h, &b.arr_m);
    if (b.arr_h > 23 || b.arr_m > 59 || n_3 < 2 || b.arr_h < 0 || b.arr_m < 0)
    {
        printf("Invalid input.\n");
        return 0;
    }
    printf("Train B departure time:\n");
    int n_4 = scanf("%d:%d", &b.depp_h, &b.depp_m);
    if (b.depp_h > 23 || b.depp_m > 59 || n_4 < 2 || b.depp_h < 0 || b.depp_m < 0)
    {
        printf("Invalid input.\n");
        return 0;
    }
    
    printf("Train C arrival time:\n");
    int n_5 = scanf("%d:%d", &c.arr_h, &c.arr_m);
    if (c.arr_h > 23 || c.arr_m > 59 || n_5 < 2 || c.arr_h < 0 || c.arr_m < 0)
    {
        printf("Invalid input.\n");
        return 0;
    }
    printf("Train C departure time:\n");
    int n_6 = scanf("%d:%d", &c.depp_h, &c.depp_m);
    if (c.depp_h > 23 || c.depp_m > 59 || n_6 < 2 || c.depp_h < 0 || c.depp_m < 0)
    {
        printf("Invalid input.\n");
        return 0;
    }

//check transfer from A
bool a_to_b = change(a,b);
bool a_to_c = change(a,c);
if (a_to_b == true && a_to_c == true)
{
    printf("Can change to both B and C from A.\n");
}
else
{
    if (a_to_b == true)
    printf("Can change to B from A.\n");
    else
    if (a_to_c == true)
    printf("Can change to C from A.\n");
    else
    printf("No changes available from train A.\n");
}

//check transfer from B
bool b_to_a = change(b,a);
bool b_to_c = change(b,c);
if (b_to_a == true && b_to_c == true)
{
    printf("Can change to both A and C from B.\n");
}
else
{
    if (b_to_a == true)
    printf("Can change to A from B.\n");
    else
    if (b_to_c == true)
    printf("Can change to C from B.\n");
    else
    printf("No changes available from train B.\n");
}
//check transfer from C
bool c_to_a = change(c,a);
bool c_to_b = change(c,b);
if (c_to_a == true && c_to_b == true)
{
    printf("Can change to both A and B from C.\n");
}
else
{
    if (c_to_a == true)
    printf("Can change to A from C.\n");
    else
    if (c_to_b == true)
    printf("Can change to B from C.\n");
    else
    printf("No changes available from train C.\n");
}
}

bool change(train from, train to)
{
    if (from.arr_h == 0)
    from.arr_h = 24;
    if (from.depp_h == 0)
    from.depp_h = 24;
    if (to.arr_h == 0)
    to.arr_h = 24;
    if (to.depp_h == 0)
    to.depp_h = 24;
    int t_1 = from.arr_h * 60 + from.arr_m;
    int t_2 = to.depp_h * 60 + to.depp_m;
    int t_3 = to.arr_h * 60 + to.arr_m;
    if (t_2 - t_1 >= 5 && t_3 - t_1 <= 3 * 60)
    return true;
    else
    return false;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max_length_to_words 1000
#define max_size_to_array 1000
typedef struct QueueNode
{
    char token[max_length_to_words];
    int index;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *front;
    QueueNode *rear;
} Queue;
typedef struct StackNode
{
    char* operation; // Insert or Remove
    char* token;
    int index;
    struct StackNode* next;
} stackNode;

typedef struct Stack
{
    stackNode* top;
} stack;
void display_menu();
void load_data(char *initial_Text);
void print_loaded_file(char* our_Text);
void insert_new_text(char *our_Text,stack* undo_stack);
void enqueue(Queue* q,char* token);
void dequeue(Queue* q);
void split_string(char *string1, char *string2);
void add_into_index(char *text,int length,char* new_word,int index);
int is_spaces(char* str);
void push(stack *s, char *operation, char *token, int index);
void pop(stack *s);
int is_empty(stack *s);
void delete_user_text(char *our_text,stack* undo_stack);
int exisit_delet(char *copy_arr,char *deleted_text);
void remove_text(char *copy_arr,char *deleted_text,int index);
void Perform_Undo(char *our_Text,stack* undo_stack,stack* redo_stack);
void print_stack(stack *s);
void Perform_redo(char *our_Text,stack* undo_stack,stack* redo_stack);
void save_output_file(char* our_Text);
int main()
{
    int option;
    stack* undo_stack=(stack*)malloc(sizeof(stack));
    stack* redo_stack=(stack*)malloc(sizeof(stack));
    char our_Text[max_size_to_array];
    strcpy(our_Text,"");
    int flag_loded=0;
    if (undo_stack == NULL)
    {
        printf("no memory.\n");
        return 0;
    }
    undo_stack->top=NULL;
    if (redo_stack == NULL)
    {
        printf("no memory.\n");
        return 0;
    }
    redo_stack->top=NULL;

    do
    {
        display_menu();
        scanf("%d", &option);
        getchar();
        switch (option)
        {
        case 1:
            if(flag_loded==0)
            {
                load_data(our_Text);
                flag_loded=1;
            }
            else
            {
                printf("sorry but,we loaded the data before this time!!");

            }
            break;
        case 2:
            if(flag_loded==0)
            {
                printf("please loaded data before this option");
            }
            else
            {
                print_loaded_file(our_Text);
            }
            break;
        case 3:
            if(flag_loded==0)
            {
                printf("please loaded data before this option");
            }
            else
            {
            insert_new_text(our_Text,undo_stack);
            }
            break;
        case 4:
            if(flag_loded==0)
            {
                printf("please loaded data before this option");
            }
            else
            {
            delete_user_text(our_Text,undo_stack);
            }
            break;
        case 5:
            Perform_Undo(our_Text,undo_stack,redo_stack);
            break;
        case 6:
            Perform_redo(our_Text,undo_stack,redo_stack);
            break;
        case 7:
            printf("the undo stack:\n");
            print_stack(undo_stack);
            printf("the redo stack:\n");
            print_stack(redo_stack);
            break;
        case 8:
            save_output_file(our_Text);
            break;
        case 9:
            break;
        default:
            printf("Invalid option! Please try again.\n");
            break;
        }
        printf("\n\n");
    }
    while (option != 9);
    return 0;
}
void display_menu()
{
    printf("Enter your operation:\n");
    printf("1. Load input file\n");
    printf("2. Print loaded text\n");
    printf("3. Insert string\n");
    printf("4. Remove strings from the text.\n");
    printf("5. Perform Undo operation\n");
    printf("6. Perform Redo operation\n");
    printf("7. Print undo and redo stacks\n");
    printf("8. Save the updated text to the output file\n");
    printf("9. Exit\n");
}
void load_data(char *initial_Text)
{
    FILE *input;
    input = fopen("input.txt", "r");
    if (input == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }
    char initial_text[max_size_to_array];
    while (fgets(initial_text, sizeof(initial_text), input) != NULL)
    {
        strcat(initial_Text,initial_text);//we are countinuo in the exisit string
    }
    fclose(input);
    printf("done loaded file successfully");

}
void print_loaded_file(char* our_Text)
{
    printf("%s",our_Text);

}
void insert_new_text(char *our_Text,stack* undo_stack)
{
    Queue* q = (Queue*)malloc(sizeof(Queue)); // Allocate memory for the queue
    if (q == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(0);
    }
    q->front = NULL;
    q->rear = NULL;

    char new_text[max_size_to_array];
    new_text[0]='\0';//to chead if the user input a true value
    int from_index;
    printf("Please enter the new text to insert: ");
    gets(new_text);
    while(new_text[0]=='\0') //if the user input empty value i have to reperet the input after cheak if he want to continuo or not
    {
        int option;
        printf("error input please select an option\n1-insert new to the text\n2-go out insert opreation(back into menu)\n");

        while(1)
        {
            scanf("%d",&option);
            if(option==1||option==2)
                break;
            printf("seclect correct opetion please 1 or 2 \n1-insert new to the text\n2-go out insert opreation(back into menu)\n");
        }
        if(option==1)
            return;
        if(option==2)
        {
            printf("Please enter the new text to insert: ");
            getchar();//beacause the prev get char we use it on other gets we need to write it here again or we cant take the data from the use.
            gets(new_text);
        }
    }
    if(is_spaces(new_text)==0) //cheak if i need add all spaces or not this is the save way to insert all spaces and dont loss any one
    {
        //after cheak insert the array into the queue
        char temp1[max_size_to_array];
        char temp2[max_size_to_array];
        char to_spaces[max_size_to_array];//to save the space in the first of user input
        char forcopy[max_size_to_array];//for remove the warning when we use strcpy of
        strcpy(forcopy,new_text);
        strcpy(temp1, "");
        strcpy(temp2, "");
        strcpy(to_spaces, "");
        strcpy(temp1,new_text);//to use it in my split function
        int x=0;
        for(; new_text[x]==' '; x++);
        strcpy(new_text,forcopy+x);
        while(x>0)
        {
            strcat(to_spaces," ");
            x--;
        }
        strcpy(temp1,new_text);
        split_string(temp1,temp2);
        strcat(to_spaces,temp2);
        strcpy(temp2,to_spaces);
        char* token = strtok(new_text, " ");//to determin where i should stop
        while (token != NULL)
        {
            enqueue(q,temp2);
            token = strtok(NULL, " ");
            split_string(temp1,temp2);
        }

    }
    else
    {
        enqueue(q,new_text);
    }
    int our_text_length=strlen(our_Text);
    printf("please enter the index to add text after it\n (if you need to add after the current text please enter -1)\n\"notefirst char is index is 1\":\n");
    scanf("%d",&from_index);
    int index=from_index;
    if(from_index>our_text_length||(from_index<0&&from_index!=-1))
    {
        printf("error the max index is %d and is cant be less than 1",our_text_length);
        return;
    }
    if(from_index==-1)
    {

        from_index=our_text_length;
        if(our_Text[from_index-1]=='\n') //to add in the end of the last line not on the new line
        {
            from_index--;
        }

    }

    while (q->front != NULL)
    {
        add_into_index(our_Text,our_text_length,q->front->token,from_index);
        push(undo_stack,"Insert" ,q->front->token,index);
        our_text_length+=strlen(q->front->token);
        from_index+=strlen(q->front->token);
        dequeue(q);//the eque every time add into the rear and server the front
    }
    printf("done added your text successfully");

}
void enqueue(Queue* q,char* token)
{
    QueueNode* new_node = (QueueNode*)malloc(sizeof(QueueNode));
    if (new_node == NULL)
    {
        printf("no memory.\n");
        exit(0);
    }
    strcpy(new_node->token, token);
    new_node->next = NULL;

    if (q->front == NULL)
    {
        q->front = new_node;
        q->rear = new_node;
    }
    else
    {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}
void dequeue(Queue* q)
{
    if (q->front == NULL)
    {
        printf("Queue is empty.\n");
        exit(0);
    }

    QueueNode* temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    free(temp);
}
void split_string(char *string1, char *string2)
{
//beacue the strtok function doenot save the spaces between word and cut the word in wrong way in beld this function
    char temp[max_length_to_words];
    int i = 0;
    for (i = 0; i <strlen(string1); i++)
    {
        if(string1[i]=='\0')
            break;
        if (string1[i]==' '&&string1[i+1]!=' ')
            break;
    }
    i++;
    strncpy(temp,string1,i);
    temp[i] = '\0';//strncpy doescot add null terminal iadd it
    strcpy(string2, temp);
    strcpy(string1, string1 + i);//return new str1 whic start from the last index we stop on split it
}
void add_into_index(char text[max_size_to_array],int length,char* new_word,int index)
{
    int new_word_length=strlen(new_word);
    for(int i=length; i>=index; i--) //shifting the char and spaces from the index to be more ready for new text
    {
        text[i+new_word_length]=text[i];
    }
    for (int i=0; i<new_word_length; i++)
    {
        text[index+ i]=new_word[i];
    }
    text[length+new_word_length]='\0';
}
int is_spaces(char* str)
{
    int i=0;
    while (str[i] != '\0')
    {if (str[i] !=' ')
        {return 0;
            // If the character is not a space, return 0
            }
        i++;
    }
    return 1;// If all characters are spaces, return 1
}
void push(stack *s, char *operation, char *token, int index)
{
    stackNode *newNode = (stackNode *)malloc(sizeof(stackNode));
    if (newNode == NULL)
    {
        printf("no memory failed.\n");
        exit(1);
    }
    newNode->operation =strdup(operation);// for memory and copy the operation string
    newNode->token=strdup(token);//for memory and copy the token string strdup is take the right size of the array
    newNode->index =index;
    newNode->next=s->top;
    s->top = newNode;
}

void pop(stack *s)
{
    if (is_empty(s))
    {
        printf("Stack is empty. Cannot pop.\n");
        return;
    }
    stackNode *temp = s->top;
    s->top = s->top->next;
    free(temp->operation); // Free the memory allocated for the operation string
    free(temp->token);     // Free the memory allocated for the token string
    free(temp);
}
int is_empty(stack *s)
{
    return s->top == NULL;//1 empty 0 not
}
void delete_user_text(char *our_text,stack* undo_stack)
{
    char deleted_text[strlen(our_text)];//the max thing ican delet it is the all text
    printf("Please enter the new text to delet: ");
    gets(deleted_text);
    int index=exisit_delet(our_text,deleted_text);
    if(index==-1)
    {
        printf("error!!!\nsorry but your input text doesnot exist on our text\n");
        return;
    }
    remove_text(our_text,deleted_text,index);
    push(undo_stack,"remove",deleted_text,index);
    printf("done delet the text successfully");

}
void remove_text(char *our_text,char *deleted_text,int index)//i take the index i have it form exist function and after iam do the shift to delet the user text
{
    int i;
    int delet_length=strlen(our_text)-strlen(deleted_text);
    for(i=index; i<(delet_length); i++)
    {
        our_text[i]=our_text[i+strlen(deleted_text)];
    }
    our_text[i]='\0';
}
int exisit_delet(char *our_text,char *deleted_text)
{
    int deleted_text_len = strlen(deleted_text);
    int max_postion_todelet=strlen(our_text) - deleted_text_len;
    for (int i = 0; i<=max_postion_todelet; i++)//to reach of element element and see if it exist in my text or no
    {
        int j;
        for (j = 0; j < deleted_text_len; j++)
        {
            if (our_text[i+j]!=deleted_text[j])//if any not occure in this words break to try star with the next char
            {
                break;
            }
        }
        if (j == deleted_text_len)//j==dele len that mean in char with postion i it is have the same char with the text ineed to del it
        {
            return i;
        }
    }
    return -1;
}
void Perform_Undo(char *our_text,stack* undo_stack,stack* redo_stack)
{
    if(is_empty(undo_stack))
    {
        printf("no undo to do it");
        return;
    }
    char undo_text[strlen(undo_stack->top->token)];//save the text and index that we do last op in it to push it into redo stack
    int index=undo_stack->top->index;

    strcpy(undo_text,undo_stack->top->token);
    char opposite_operation[10];
    strcpy(opposite_operation,"Insert");//the defult operation we have it is insert
    if(strcmp((undo_stack->top->operation),opposite_operation)==0) //if we have in undo insert we convert it to remove and the oppsite is right
    {
        if(index==-1){
        index=strlen(our_text)-strlen(undo_stack->top->token)-1;
    }
        remove_text(our_text,undo_text,index);
    }
    else
    {
        if(index==-1){
        index=strlen(our_text)-1;
    }

        strcpy(opposite_operation,"remove");
        add_into_index(our_text,strlen(our_text),undo_text,index);
    }
    push(redo_stack,opposite_operation,undo_text,undo_stack->top->index);
    pop(undo_stack);
    printf("done perform undo successfully");

}
void Perform_redo(char *our_text,stack* undo_stack,stack* redo_stack)
{
    if(is_empty(redo_stack))
    {
        printf("no redo to do it");
        return;
    }
    char redo_text[strlen(redo_stack->top->token)];
    int index=redo_stack->top->index;

    strcpy(redo_text,redo_stack->top->token);
    char opposite_operation[10];
    strcpy(opposite_operation,"remove");//if we have insert in redo that mean thee word it is remvo when we click perfuom undo
    if(strcmp((redo_stack->top->operation),opposite_operation)==0) //if we have in undo insert we convert it to remove and the oppsite is right
    {
        if(index==-1){

        index=strlen(our_text)-strlen(redo_stack->top->token)-2;//if we add in the last index of the stack is -1 so heere if found the right postion to delet it if we undo operation
    }

        remove_text(our_text,redo_text,index);
    }
    else
    {
        strcpy(opposite_operation,"insert");
        if(index==-1){
        index=strlen(our_text)-1;
    }
        add_into_index(our_text,strlen(our_text),redo_text,index);
    }
    push(undo_stack,opposite_operation,redo_text,redo_stack->top->index);
    pop(redo_stack);
   printf("done perform redo successfully");


}
void print_stack(stack *s)
{
    if (is_empty(s))
    {
        printf("Stack is empty.\n");
        return;
    }
    stackNode *temp = s->top;
    while (temp != NULL)
    {
        printf("Operation: %s, Token: %s, Index: %d\n", temp->operation, temp->token, temp->index);
        temp = temp->next;
    }
}
void save_output_file(char* our_Text)
{
    FILE *output;
    output = fopen("output.txt", "w");
    if (output == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }
        fprintf(output,"%s",our_Text);
printf("done store the text in output file  successfully");
}

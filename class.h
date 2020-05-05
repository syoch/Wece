#ifndef CLASS_H
#define CLASS_H

#define ClassNewHeader(class)\
    class *object;\
    object = malloc(sizeof(class));\
    if (object == NULL){\
        printf("%4s New: Failed to make Class of '%s'\n",#class,#class);\
        exit(1);\
    }\
    memset(object, 0, sizeof(class));
#define Class(variablename, Classname, args...) \
    Classname *variablename;                    \
    variablename = Classname##_new(args);
#define ClassList(variablename, Classname, length, args...) \
    Classname *variablename[length];                        \
    for (int cil = 0; cil < length; cil++)                  \
    {                                                       \
        variablename[cil] = Classname##_new(args);          \
    }


#endif
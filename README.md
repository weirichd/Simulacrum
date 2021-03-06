![status badge](https://travis-ci.org/ztbrown/Simulacrum.svg?branch=master)
# Simulacrum

A test double framework written in C

## Prerequisites 
- gcc
- Make
- c99
- Docker (optional) 

## Usage

### Replace a function call with a noop / NULL return

```C

SIMULACRUM(void *, malloc, 1, size_t)

START_TEST(noop_malloc)
{
  char *actual = NULL;
  
  actual = malloc(16 * sizeof(char));
  
  ck_assert_ptr_eq(actual, NULL);
}
END_TEST

```
### Keep track of call count

```C

SIMULACRUM(void *, calloc, 2, size_t, size_t)

START_TEST(it_calls_calloc_twice)
{
    char *str = NULL;
    char *str2 = NULL;
    
    str = calloc(16, sizeof(char));
    str2 = calloc(16, sizeof(char));
    
    ck_assert_int_eq(mock_get_call_count(&calloc_mock), 2);
}
END_TEST

```

### Add a callback / Set a return value 

```C

SIMULACRUM(char *, fgets, 3, char *, int, void *)

void fill_out_param_callback(char *buf, int size, void *file)
{
    char *mock_file_line = "A line in a fake file";
    memcpy(buf, mock_file_line, size);
}

START_TEST(it_uses_a_callback_to_set_out_param)
{
    int size = 32;
    char buf[48];
    char *rtn = "hello world";
    char *actual;

    mock_set_callback(&fgets_mock, &fill_out_param_callback);
    mock_set_return_value(&fgets_mock, &rtn);

    actual = fgets(buf, size, NULL);

    ck_assert_str_eq(actual, "hello world");
    ck_assert_str_eq(buf, "A line in a fake file");
}
END_TEST

```

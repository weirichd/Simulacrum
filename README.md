![status badge](https://travis-ci.org/ztbrown/Simulacrum.svg?branch=master)
# Simulacrum
## A test double framework in C

7th-level illusion

Casting time: Instantaneous 

Range: Touch

Components: V, S, M (snow or ice in quantities sufficient to make a life-size copy of the duplicated creature; some hair, fingernail clippings, or other piece of that creature's body placed inside the snow or ice; and powdered ruby worth 1,500 gp, sprinkled over the duplicate and consumed by the spell)

Duration: Until dispelled

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

### Add a callback

```C

SIMULACRUM(void, fgets, 3, char *, int, void *)

void fill_out_param_callback(char *buf, int size, void *file)
{
    char *mock_file_line = "A line in a fake file";
    memcpy(buf, mock_file_line, size);
}

START_TEST(it_uses_a_callback_to_set_out_param)
{
    int size = 32;
    char buf[48];

    mock_set_callback(&fgets_mock, &fill_out_param_callback);

    fgets(buf, size, NULL);

    ck_assert_str_eq(buf, "A line in a fake file");
}
END_TEST

```

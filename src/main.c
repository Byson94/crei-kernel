void print_string(char *string_ptr) {
    char *str = string_ptr;
    
    volatile char *video_memory = (volatile char*) 0xB8000;
    
    int i = 0;
    while (str[i] != '\0') {
        video_memory[i * 2] = str[i];
        video_memory[i * 2 + 1] = 0x0F;
        
        i++;
    }
}

int main() {
    print_string("Hello from Crei!");
    while(1) {}

    return 0;
}

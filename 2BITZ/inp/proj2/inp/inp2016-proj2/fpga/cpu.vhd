-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2016 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): DOPLNIT
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- defining my data tyypes
 
    type mx_sel_types is (
        mx_in_data, mx_tmp_out, mx_data_rdata_plus, mx_data_rdata_minus 
    );
    
    type fsm_states is (
        inc_val0, inc_val1, dec_val0, dec_val1, inc_ptr, dec_ptr, start_cycle0, start_cycle1, start_cycle2,
        start_cycle3, end_cycle0, end_cycle1, end_cycle2, end_cycle3, end_cycle4, print0, print1,
        getchar, store, load0, load1, init, fetch, decode, halt,
        unknown
    );

-- SIGNALS
 -- PC register signals
    signal pc_inc   :   std_logic;
    signal pc_dec   :   std_logic;
    signal pc_out   :   std_logic_vector (11 downto 0);
    
 -- CNT register signals
    signal cnt_inc  :   std_logic;
    signal cnt_dec  :   std_logic;
    signal cnt_out  :   std_logic_vector (7 downto 0);
    
 -- TMP register signals
    signal tmp_ld   :   std_logic;
    signal tmp_in   :   std_logic_vector (7 downto 0);
    signal tmp_out  :   std_logic_vector (7 downto 0);
    
 -- PTR register signals
    signal ptr_inc  :   std_logic;
    signal ptr_dec  :   std_logic;
    signal ptr_out  :   std_logic_vector (9 downto 0);
    
 -- MX signals
    signal mx_sel   :   mx_sel_types;
    signal mx_out   :   std_logic_vector (7 downto 0);
    signal mx_in_plus   :   std_logic_vector (7 downto 0);
    signal mx_in_minus  :   std_logic_vector (7 downto 0);
    
 -- fsm states
    signal current_state    :   fsm_states;
    signal next_state       :   fsm_states;
 
begin

 -- connecting parts together
 
    --mulitplexor input
    mx_in_plus  <=  DATA_RDATA + '1';   --TODO check maybe + "00000001"
    mx_in_minus <=  DATA_RDATA - '1';
    --multiplexor output
    DATA_WDATA  <=  mx_out;
    
 -- connecting register tmp to real world
    tmp_in      <=  DATA_RDATA;
 
    CODE_ADDR   <=  pc_out;         -- program coutner
    DATA_ADDR   <=  ptr_out;        -- ptr to ram  
    OUT_DATA    <=  DATA_RDATA;     -- connecting memory to display
    
 -- processes of cpu

    pc_register :   process(CLK, RESET)
        begin
            if ( RESET = '1' ) then
                pc_out <= ( others => '0' );
            elsif ( CLK'event ) and ( CLK = '1' ) then 
                if ( pc_inc = '1' ) then
                    pc_out <= pc_out + 1;
                elsif ( pc_dec = '1' ) then
                    pc_out <= pc_out - 1;
                end if;
            end if;
        end process;
        
    cnt_register :  process(CLK, RESET)
        begin
            if ( RESET = '1' ) then
                cnt_out <= ( others => '0' );
            elsif ( CLK'event ) and ( CLK = '1' ) then 
                if ( cnt_inc = '1' ) then
                    cnt_out <= cnt_out + 1;
                elsif ( cnt_dec = '1' ) then
                    cnt_out <= cnt_out - 1;
                end if;
            end if;
        end process;
        
    tmp_register :      process(CLK, RESET, tmp_ld, tmp_in)
        begin
            if ( RESET = '1' ) then
                tmp_out <= ( others => '0' );
            elsif ( CLK'event and CLK = '1') then
                if ( tmp_ld = '1' ) then
                    tmp_out <= tmp_in;
                end if;
            end if;
        end process;
 
    ptr_register :      process(CLK, RESET)
        begin
            if ( RESET = '1' ) then
                ptr_out <= ( others =>'0');
            elsif ( CLK'event ) and ( CLK = '1' ) then 
                if ( ptr_inc = '1' ) then
                    ptr_out <= ptr_out + 1;
                elsif ( ptr_dec = '1' ) then
                    ptr_out <= ptr_out - 1;
                end if;
            end if;
        end process;
        
 -- mulitplexor from exercies
    with mx_sel select
        mx_out  <=  IN_DATA     when    mx_in_data,
                    tmp_out     when    mx_tmp_out,
                    mx_in_plus  when    mx_data_rdata_plus,
                    mx_in_minus when    mx_data_rdata_minus;
                    
    
 -- instruction adder, it chagnes instruction on CLK'EVENT, good for synchronization
    instr_adder :       process(CLK, RESET)
        begin
            if ( RESET = '1' ) then
                current_state <= init;
            elsif ( CLK'event ) and ( CLK = '1' ) then
                if ( EN = '1' ) then
                    current_state <= next_state;
                end if;
            end if;
        end process;
    
 -- FSM
    fsm :               process(CLK, CODE_DATA, DATA_RDATA, cnt_out, OUT_BUSY, IN_VLD, current_state)
        begin
        -- initializing default values
            
            next_state  <=  fetch;
        
            pc_inc  <=  '0';
            pc_dec  <=  '0';
            cnt_inc <=  '0';
            cnt_dec <=  '0';
            tmp_ld  <=  '0';
            ptr_inc <=  '0';
            ptr_dec <=  '0';
            
        -- default values for peripheral devices
            DATA_RDWR   <=  '1';
            DATA_EN <=  '0';
            CODE_EN <=  '0';
            OUT_WE  <=  '0';
            IN_REQ  <=  '0';
            mx_sel  <=  mx_in_data;
            
        -- FSM 
            case    current_state   is
                when    init        =>
----------------------------------------------------------------------------------                
                when    fetch       =>
                    CODE_EN     <=  '1';
                    next_state  <=  decode;
----------------------------------------------------------------------------------                
                when    decode      =>
                    case    CODE_DATA   is
                        when    X"3E"       =>
                            next_state  <=  inc_ptr;
                        
                        when    X"3C"       =>
                            next_state  <=  dec_ptr;
                        
                        when    X"2B"       =>
                            next_state  <=  inc_val0;
                        
                        when    X"2D"       =>
                            next_state  <=  dec_val0;
                        
                        when    X"5B"       =>
                            next_state  <=  start_cycle0;
                        
                        when    X"5D"       =>
                            next_state  <=  end_cycle0;
                        
                        when    X"2E"       =>
                            next_state  <=  print0;
                        
                        when    X"2C"       =>
                            next_state  <=  getchar;
                        
                        when    X"24"       =>
                            next_state  <=  load0;
                        
                        when    X"21"       =>
                            next_state  <=  store;
                        
                        when    X"00"       =>
                            next_state  <=  halt;
                            
                        when others         =>
                            next_state  <=  unknown;
                    end case;
----------------------------------------------------------------------------------                
                when    inc_ptr     =>
                    ptr_inc     <=  '1';
                    pc_inc      <=  '1';
----------------------------------------------------------------------------------                
                when    dec_ptr     =>
                    ptr_dec     <=  '1';
                    pc_inc      <=  '1';
----------------------------------------------------------------------------------                
                when    inc_val0    =>
                    DATA_EN     <=  '1';
                    next_state  <=  inc_val1;
----------------------------------------------------------------------------------         
                when    inc_val1    =>
                    DATA_RDWR   <=  '0';
                    DATA_EN     <=  '1';
                    mx_sel      <=  mx_data_rdata_plus;
                    pc_inc      <=  '1';
----------------------------------------------------------------------------------         
                when    dec_val0    =>
                    DATA_EN     <=  '1';
                    next_state  <=  dec_val1;                
----------------------------------------------------------------------------------         
                when    dec_val1    =>
                    DATA_RDWR   <=  '0';
                    DATA_EN     <=  '1';
                    mx_sel      <=  mx_data_rdata_minus;
                    pc_inc      <=  '1';
----------------------------------------------------------------------------------                
                when    start_cycle0 =>
                    pc_inc  <=  '1';
                    DATA_EN <=  '1';
                    next_state  <=  start_cycle1;
----------------------------------------------------------------------------------                
                when    start_cycle1 =>                                        
                    if ( DATA_RDATA = "00000000" ) then
                        cnt_inc <=  '1';
                        next_state  <=  start_cycle2;
                    end if;
----------------------------------------------------------------------------------                
                when    start_cycle2 =>              
                    if ( cnt_out = "00000000" ) then
                    
                    else
                        CODE_EN <=  '1';
                        next_state  <=  start_cycle3;
                    end if;
----------------------------------------------------------------------------------                
                when    start_cycle3 =>              
                    if ( CODE_DATA = X"5B" ) then
                        cnt_inc <=  '1';
                    elsif ( CODE_DATA = X"5D") then
                        cnt_dec <=  '1';
                    end if;
                    
                    pc_inc      <=  '1';
                    next_state  <=  start_cycle2;            
----------------------------------------------------------------------------------                
                when    end_cycle0   =>
                    DATA_EN <=  '1';
                    next_state  <=  end_cycle1;
----------------------------------------------------------------------------------                
                when    end_cycle1   =>
                    if ( DATA_RDATA = "00000000" ) then
                        pc_inc  <=  '1';
                    else
                        cnt_inc <=  '1';
                        pc_dec  <=  '1';
                        next_state  <=  end_cycle2;
                    end if;
----------------------------------------------------------------------------------                
                when    end_cycle2   =>
                    if ( cnt_out = "00000000" ) then
                    else
                        CODE_EN <=  '1';
                        next_state  <=  end_cycle3;
                    end if;
----------------------------------------------------------------------------------                
                when    end_cycle3   =>
                    if ( CODE_DATA = X"5D" ) then
                        cnt_inc <=  '1';
                    elsif ( CODE_DATA = x"5B" ) then
                        cnt_dec <=  '1';
                    end if;
                    next_state  <=  end_cycle4;
----------------------------------------------------------------------------------                
                when    end_cycle4   =>
                    if ( cnt_out = "00000000" ) then
                        pc_inc  <=  '1';
                    else
                        pc_dec  <= '1';
                    end if;
                    next_state <= end_cycle2;
  
                when    print0      =>
                    DATA_EN <=  '1';
                    next_state  <=  print1;
----------------------------------------------------------------------------------                
                when    print1      =>
                    if ( OUT_BUSY = '0' ) then
                        OUT_WE  <=  '1';
                        pc_inc  <=  '1';
                    else
                        next_state  <=  print1;
                    end if;
----------------------------------------------------------------------------------                
                when    getchar     =>
                    IN_REQ  <=  '1';
                    mx_sel  <=  mx_in_data;
                    if ( IN_VLD = '1' ) then
                        DATA_RDWR   <=  '0';
                        DATA_EN     <=  '1';
                        pc_inc      <=  '1';
                    else
                        next_state  <=  getchar;
                    end if;
----------------------------------------------------------------------------------                
                when    store       =>
                    mx_sel  <=  mx_tmp_out;
                    DATA_RDWR   <=  '0';
                    DATA_EN     <=  '1';
                    pc_inc      <=  '1';
                    
----------------------------------------------------------------------------------                
                when    load0       =>
                    DATA_EN     <=  '1';
                    next_state  <=  load1;

----------------------------------------------------------------------------------                
                when    load1       =>
                    tmp_ld  <=  '1';
                    pc_inc  <=  '1';
----------------------------------------------------------------------------------                
                when    halt        =>
                    next_state  <=  halt;
----------------------------------------------------------------------------------                
                when    unknown     =>
                    pc_inc  <=  '1';
                    
            end case;
        end process;
        
end behavioral;
 

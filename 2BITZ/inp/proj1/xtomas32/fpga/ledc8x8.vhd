library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( 
    ROW: out std_logic_vector(7 downto 0);
    LED: out std_logic_vector(7 downto 0);
    RESET: in std_logic;
    SMCLK: in std_logic
);
end entity ledc8x8;

architecture main of ledc8x8 is

    -- Sem doplnte definice vnitrnich signalu.
    
    signal LETTER_A: std_logic_vector(7 downto 0);
    signal LETTER_T: std_logic_vector(7 downto 0);
    signal MY_CLK: std_logic;
    signal CNT: std_logic_vector(3 downto 0);
    signal MY_CLK_CNT: std_logic_vector(24 downto 0);
    signal LETTER_CNT: std_logic_vector(2 downto 0);
    
    

begin


    MY_CLK_P: process(RESET,SMCLK)
    begin
        if (RESET = '1') then 
            MY_CLK <= '1';
            MY_CLK_CNT(7 downto 0) <= "00000000";
        elsif (SMCLK = '1') and (SMCLK'event) then
            MY_CLK_CNT <= MY_CLK_CNT + '1';
        end if;

    end process MY_CLK_P;
    
    ROW_REG: process(RESET,MY_CLK)
    begin
        if (RESET = '1') then
            ROW <= "11111110";
        elsif (MY_CLK'event) and (MY_CLK = '1') then
            case MY_CLK_CNT(10 downto 8) is 
                when "000" => ROW <= "00000001";
                when "001" => ROW <= "00000010";
                when "010" => ROW <= "00000100";
                when "011" => ROW <= "00001000";
                when "100" => ROW <= "00010000";
                when "101" => ROW <= "00100000";
                when "110" => ROW <= "01000000";
                when "111" => ROW <= "10000000";
                when others => ROW <="00000000";
            end case;
        end if;
        
    end process ROW_REG;
    
    LETTER_CNT_P: process(RESET,MY_CLK)
    begin
        if (RESET = '1') then
            LETTER_CNT <= "000";
        elsif (MY_CLK'event) and (MY_CLK = '1') then
            LETTER_CNT <= LETTER_CNT + '1';
        end if;
    end process LETTER_CNT_P;
    
    LETTER_A_P: process(MY_CLK)
    begin
        if (MY_CLK'event) and (MY_CLK = '1') then
            case MY_CLK_CNT(10 downto 8) is 
                when "000" => LETTER_A <= "11100111";
                when "001" => LETTER_A <= "11011011";
                when "010" => LETTER_A <= "10111101";
                when "011" => LETTER_A <= "10111101";
                when "100" => LETTER_A <= "10000001";
                when "101" => LETTER_A <= "10111101";
                when "110" => LETTER_A <= "10111101";
                when "111" => LETTER_A <= "10111101";
                when others => LETTER_A <=  "00000000";
            end case;
        
        end if;
    end process LETTER_A_P;
    
        LETTER_T_P: process(MY_CLK)
    begin
        if (MY_CLK'event) and (MY_CLK = '1') then
            case MY_CLK_CNT(10 downto 8) is 
                when "000" => LETTER_T <= "00000000";
                when "001" => LETTER_T <= "00000000";
                when "010" => LETTER_T <= "11100111";
                when "011" => LETTER_T <= "11100111";
                when "100" => LETTER_T <= "11100111";
                when "101" => LETTER_T <= "11100111";
                when "110" => LETTER_T <= "11100111";
                when "111" => LETTER_T <= "11100111";
                when others => LETTER_T <="00000000";
            end case;
        
        end if;
    end process LETTER_T_P;
    
    LETTER_SW: process
    begin
        case MY_CLK_CNT(23) is 
            when '0' => LED <= LETTER_A;
            when '1' => LED <= LETTER_T;
            when others => LED <= LETTER_A;
        end case;
    end process LETTER_SW;
    
end main;

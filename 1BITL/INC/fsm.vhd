-- fsm.vhd: Finite State Machine
-- Author(s): xtomas32
--
library ieee;
use ieee.std_logic_1164.all;
-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity fsm is
port(
   CLK         : in  std_logic;
   RESET       : in  std_logic;

   -- Input signals
   KEY         : in  std_logic_vector(15 downto 0);
   CNT_OF      : in  std_logic;

   -- Output signals
   FSM_CNT_CE  : out std_logic;
   FSM_MX_MEM  : out std_logic;
   FSM_MX_LCD  : out std_logic;
   FSM_LCD_WR  : out std_logic;
   FSM_LCD_CLR : out std_logic
);
end entity fsm;

-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of fsm is
   type t_state is (TEST1,TEST2,TEST3,TEST4,TEST5,TEST6,TEST7,TEST8a,TEST9a,TEST10a,
   TEST11a,TEST8b,TEST9b,TEST10b,TEST11b,PRINT_TRUE,PRINT_FALSE,ERROR_STATE, FINISH);
   signal present_state, next_state : t_state;

begin
-- -------------------------------------------------------
sync_logic : process(RESET, CLK)
begin
   if (RESET = '1') then
      present_state <= TEST1;
   elsif (CLK'event AND CLK = '1') then
      present_state <= next_state;
   end if;
end process sync_logic;

-- -------------------------------------------------------
next_state_logic : process(present_state, KEY, CNT_OF)
begin
   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1 =>
      next_state <= TEST1;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(1) = '1') then
	  next_state <= TEST2;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2 =>
      next_state <= TEST2;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(2) = '1') then
	  next_state <= TEST3;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST3 =>
      next_state <= TEST3;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(0) = '1') then
	  next_state <= TEST4;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;	
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST4 =>
      next_state <= TEST4;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(0) = '1') then
	  next_state <= TEST5;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST5 =>
      next_state <= TEST5;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(4) = '1') then
	  next_state <= TEST6;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST6 =>
      next_state <= TEST6;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(0) = '1') then
	  next_state <= TEST7;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST7 =>
      next_state <= TEST7;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(2) = '1') then
	  next_state <= TEST8a;
      elsif (KEY(0) = '1') then
	  next_state <= TEST8b;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST8a =>
      next_state <= TEST8a;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(5) = '1') then
	  next_state <= TEST9a;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST9a =>
      next_state <= TEST9a;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(6) = '1') then
	  next_state <= TEST10a;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST10a =>
      next_state <= TEST10a;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(5) = '1') then
	  next_state <= TEST11a;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST11a =>
      next_state <= TEST11a;
      if (KEY(15) = '1') then
         next_state <= PRINT_TRUE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;
      
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST8b =>
      next_state <= TEST8b;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(2) = '1') then
	  next_state <= TEST9b;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST9b =>
      next_state <= TEST9b;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(6) = '1') then
	  next_state <= TEST10b;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST10b =>
      next_state <= TEST10b;
      if (KEY(15) = '1') then
         next_state <= PRINT_FALSE;
      elsif (KEY(1) = '1') then
	  next_state <= TEST11b;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST11b =>
      next_state <= TEST11b;
      if (KEY(15) = '1') then
         next_state <= PRINT_TRUE;
      elsif (KEY(14 downto 0) /= "000000000000000") then
	  next_state <= ERROR_STATE;
      end if;
      
   -- - - - - - - - - - - - - - - - - - - - - - -
    when ERROR_STATE =>
       next_state <= ERROR_STATE;
       if (KEY(15) = '1') then
	  next_state <= PRINT_FALSE;
       end if;
         
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_TRUE =>
      next_state <= PRINT_TRUE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_FALSE =>
      next_state <= PRINT_FALSE;
      if (CNT_OF = '1') then
         next_state <= FINISH;
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      next_state <= FINISH;
      if (KEY(15) = '1') then
         next_state <= TEST1; 
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
      next_state <= TEST1;
   end case;
end process next_state_logic;

-- -------------------------------------------------------
output_logic : process(present_state, KEY)
begin
   FSM_CNT_CE     <= '0';
   FSM_MX_MEM     <= '0';
   FSM_MX_LCD     <= '0';
   FSM_LCD_WR     <= '0';
   FSM_LCD_CLR    <= '0';

   case (present_state) is
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST1 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST2 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST3 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST4 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST5 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST6 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST7 =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST8a =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST9a =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST10a =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST11a =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST8b =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST9b =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST10b =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when TEST11b =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when ERROR_STATE =>
      if (KEY(14 downto 0) /= "000000000000000") then
         FSM_LCD_WR     <= '1';
      end if;
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;

   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_FALSE =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
      FSM_MX_MEM     <= '0';
   -- - - - - - - - - - - - - - - - - - - - - - -
   when PRINT_TRUE =>
      FSM_CNT_CE     <= '1';
      FSM_MX_LCD     <= '1';
      FSM_LCD_WR     <= '1';
      FSM_MX_MEM     <= '1';

   -- - - - - - - - - - - - - - - - - - - - - - -
   when FINISH =>
      if (KEY(15) = '1') then
         FSM_LCD_CLR    <= '1';
      end if;
   -- - - - - - - - - - - - - - - - - - - - - - -
   when others =>
   end case;
end process output_logic;

end architecture behavioral;


﻿ALTER TABLE [dbo].[CSGACoupleSystemMessage] ADD
CONSTRAINT [FK_CSGACoupleSystemMessage_CSGACoupleSystem] FOREIGN KEY ([LoginUID]) REFERENCES [dbo].[CSGACoupleSystem] ([LoginUID]) ON DELETE CASCADE ON UPDATE CASCADE



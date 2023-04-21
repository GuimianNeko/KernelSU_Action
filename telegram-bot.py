import telegram

bot = telegram.Bot(token='1991981456:AAGnXsUtB54L9z_9FeiaBpezU6sAe3uNwvs')

def handle(update, context):
    print(f'message from {update.message.from_user.first_name}: {update.message.text}')
    
bot.message_handle(handle)
bot.polling()